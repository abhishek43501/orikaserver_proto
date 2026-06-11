# Connecting, Logging In, and Fetching Net Position

This guide describes the full client flow to talk to **OrikaServer**: open the
secure WebSocket, authenticate, and retrieve a user's net position.

Everything below is derived from the server source (`OrikaServer/server/Server.cpp`,
`OrikaServer/ProtoMessageConverter.cpp`, `OrikaServer/StaticClass.cpp`,
`OrikaServer/SQLDATA/SqlData.cpp`) and the `.proto` files under `ProtoFile/`.

---

## 1. Transport

| Property        | Value                                                              |
|-----------------|-------------------------------------------------------------------|
| Protocol        | Secure WebSocket (`wss://`)                                        |
| Live endpoint   | `wss://auttrading.com:85` (IP `95.111.253.230`)                   |
| Local/dev       | `wss://localhost:85`                                               |
| TLS             | TLS 1.2. The server sends the full cert chain, but the cert has chain errors against public roots — **test clients must skip/relax certificate verification.** |
| Frame type      | **Binary** WebSocket frames                                       |
| Payload         | A raw serialized protobuf `Oreka.ClientMessage` — **no** length prefix, **no** base64 |

Every message in **both** directions is a binary frame whose body is a
serialized `Oreka.ClientMessage`. There is no JSON or text on the wire.

---

## 2. The wire envelope: `Oreka.ClientMessage`

`ProtoFile/clientmessage.proto` defines one outer envelope. The two fields that
matter for this guide:

```proto
message ClientMessage {
    string                              type           = 1;   // routing string
    optional LoginRequest               loginrequest   = 2;
    optional LoginResponse              loginresponse  = 3;
    optional FetchClientPositions       fetchclientposition = 6;
    optional ClientpositionInsertandUpdate clientposition = 55;
    // ... ~90 other typed sub-messages
}
```

### How the server reads a request (important)

When a frame arrives, the server (`MessageReceived` → `convertprotoToString`):

1. Parses the bytes into a `ClientMessage`.
2. Converts it to JSON.
3. **Unwraps the envelope**: it walks the top-level members, finds the one
   non-`type` field that is an object (e.g. `loginrequest`, `fetchclientposition`),
   and treats *that inner object* as the actual message.
4. Dispatches on the inner object's own `type` field.

The practical consequence: **the routing `type` string you care about lives on
the sub-message**, and the sub-message's fields (`login`, `action`, `time`, …)
become the effective top-level fields the handler reads. Set the outer
`ClientMessage.type` to the same string for clarity, but the sub-message's `type`
is what the handler keys on after unwrapping.

### How the server sends a response

The server builds a JSON string like `{"type":"CLIENT_POSITION","insert":[...]}`,
then `JasonToProto` looks the `type` up in a table
(`m_protoandjsonmessage`, populated in `StaticClass.cpp`) to find the matching
sub-message field, re-wraps it as
`{"type":"CLIENT_POSITION","clientposition":{...}}`, serializes it to a
`ClientMessage`, and sends the bytes. Your client parses the bytes back into a
`ClientMessage` and reads the sub-message by field.

Relevant `type` → field mappings:

| `type` string     | `ClientMessage` field | Direction |
|-------------------|-----------------------|-----------|
| `login`           | `loginrequest`        | → server  |
| `LOGIN_STATUS`    | `loginresponse`       | ← server  |
| `FETCH_CLIENT_POSITIONS` | `fetchclientposition` | → server  |
| `CLIENT_POSITION` | `clientposition`      | ← server  |
| `SUBSCRIBE_TICKER` / `UNSUBSCRIBE_TICKER` | `subscribeticker` | → server  |
| `TICK_DATA`       | `tickdata`            | ← server  |
| `FETCH_ORDER_DATA` | `fetchclientposition` (generic fetch) | → server |
| `ORDER_DATA`      | `orderdata`           | ← server  |
| `FETCH_DEALING_DATA` | `fetchDealingdatainterval` (generic fetch) | → server |
| `DEALING_DATA`    | `dealingdata`         | ← server  |
| `UNSUBSCRIBE`     | `unsubscribe`         | → server  |
| `ACTIVE_COLUMNS_CHANGED` / `ACTIVE_COLUMNS_DELETE` | `activecolumnchagerequest` | → server |

---

## 3. Step 1 — Connect

1. Open a `wss://` connection to the endpoint (port **85**).
2. Disable certificate verification (the cert chain does not validate against
   public roots).
3. Keep the socket in **binary** mode.

No handshake message is required beyond the TLS/WebSocket upgrade itself.

---

## 4. Step 2 — Log in

### Request

Build and send a `ClientMessage` carrying a `LoginRequest`
(`ProtoFile/loginrequest.proto`):

```
ClientMessage {
    type: "login"
    loginrequest: LoginRequest {
        type:     "login"
        login:    "1001"          // the API login id
        pwd:      "hello12345"     // password (see note below)
        serialNo: "*"             // HD serial number, or "*" wildcard
    }
}
```

`LoginRequest` field numbers: `type=1`, `login=2`, `pwd=3`, `serialNo=4`.

**Validation order (matters for debugging):** the server validates `serialNo`
*before* the password (against `orika_userLoginHDkeyMapping`, where `HDno='*'`
is a wildcard). A bad serial short-circuits to `"invalid SerialNo"` and you
never get a password error. Logins `1001` and `1002` are exempt from HD
serial-number binding.

> **Credential note:** for login `1001` in the `orika_demo` DB the working
> password is `hello12345` (no `@`) — not the often-documented `hello@12345`.

### Response

The server replies with a `ClientMessage` of `type: "LOGIN_STATUS"` carrying a
`LoginResponse` (field `loginresponse`, number 3). Read `LoginResponse.status`:

| `status` value         | Meaning                                  |
|------------------------|------------------------------------------|
| `success`              | Authenticated. `logintoken` is populated, plus `data`. |
| `invalid credentials`  | Wrong password.                          |
| `invalid SerialNo`     | Serial not allowed for this login.       |
| `Invalid ApiLogin`     | Login id not recognized / not API-enabled. |

On success the response also includes a `logintoken` (usable later for
token-based reconnect via the `loginbytoken` flow) and a `data` payload.

### What login sets up server-side

On a successful login the server stores your client context and — critically for
the next step — populates the list of logins you are **allowed to see**
(`GetLoginsClient` + `GetUserLoginList`, `Server.cpp:520`/`:567`). Net-position
results are filtered to exactly this list. You do not subscribe to logins
separately; authentication establishes the visible set.

---

## 5. Step 3 — Fetch net position

### Request

Send a `ClientMessage` carrying a `FetchClientPositions`
(`ProtoFile/FetchClientPositions.proto`):

```
ClientMessage {
    type: "FETCH_CLIENT_POSITIONS"
    fetchclientposition: FetchClientPositions {
        type:   "FETCH_CLIENT_POSITIONS"
        action: "refresh"     // "refresh" forces a full resend; omit/"" otherwise
        time:   0             // 0 = server uses current time
    }
}
```

`FetchClientPositions` fields: `type=1`, `action=2 (string)`, `time=3 (uint32)`.

The handler (`Server.cpp:3890`) requires a valid login
(`checkLoginValidate == 1`). It immediately returns the current snapshot and
flags the context (`m_ClientWiseNetPositionStart = 1`) so that the per-client
calculation thread **keeps pushing live updates** afterward.

### Response — initial snapshot

You receive a `ClientMessage` of `type: "CLIENT_POSITION"`, with the data in the
`clientposition` field (`ClientpositionInsertandUpdate`). The snapshot arrives as
an `insert` array; each row is one login × symbol net position
(`SqlData.cpp:1847 getClientwisenetpositionData`). Fields per row:

| Field             | Meaning                                       |
|-------------------|-----------------------------------------------|
| `login`           | Account/login id                              |
| `name`            | Account holder name                           |
| `symbol`          | Instrument symbol                             |
| `volume`          | **Net position volume** (the net qty)         |
| `previousvolume`  | Prior net volume                              |
| `difference`      | Change vs. previous                           |
| `average`         | Average price of the net position             |
| `lastrate`        | Latest market rate                            |
| `clientfloatingpl`| Floating P/L                                  |
| `clientbalance`   | Account balance                               |
| `clientnetamount` | Net amount                                    |
| `maxallotedqty`   | Max allotted quantity                         |
| `clientexposure`  | Exposure (absolute value)                     |
| `extravolume`     | Extra volume                                  |
| `freemargin`      | Free margin                                   |
| `subbroker`,`broker` | Hierarchy attribution                      |

Only rows whose `login` is in your authorized list (from step 4) are included.

### Response — live updates

Because the request also starts the stream, the server then continuously pushes
further `CLIENT_POSITION` messages as the market/positions move. These use the
same `clientposition` field and carry `insert` (new rows) and/or `update`
(changed rows, matched by `updatekey`) — see `Clientposition.proto`. Keep reading
frames on the socket and apply inserts/updates to maintain a live net-position
table.

### Stopping / refreshing

- Re-send `FETCH_CLIENT_POSITIONS` with `action:"refresh"` to force a fresh full
  snapshot.
- Send an `UNSUBSCRIBE` message with `requestType:"FETCH_CLIENT_POSITIONS"` to
  stop the stream (`Server.cpp:1404`/`:1422`). The server confirms with a
  `SERVER_MESSAGE`.

---

## 6. Other live streams: subscribe ticker, orders, deals

All three of these follow the same pattern as net position: send a request that
carries a routing `type` string, the handler requires a valid login
(`checkLoginValidate == 1`), and the server then **keeps pushing** matching
frames on the same socket until you unsubscribe. As with net position, the
visible data is filtered to the login set established at authentication.

### 6.1 Subscribe to ticker (live prices)

**Request** — `SubscribeTicker` (`ProtoFile/SubscribeTicker.proto`, field
`subscribeticker`, number 46):

```
ClientMessage {
    type: "SUBSCRIBE_TICKER"
    subscribeticker: SubscribeTicker {
        type:    "SUBSCRIBE_TICKER"
        tickers: ["GOLDJUN", "SILVERMAY"]   // one or more symbols
    }
}
```

`SubscribeTicker` fields: `type=1`, `tickers=2 (repeated string)`.

The handler (`Server.cpp:4756`) records each symbol in the client context and
**immediately** sends one `TICK_DATA` snapshot per requested symbol
(`Server.cpp:4811`). After that, the tick thread streams further `TICK_DATA`
frames as quotes move.

**Response** — `type: "TICK_DATA"`, field `tickdata` (`TickData`, number 61).
The snapshot arrives as an `insert` array; live changes arrive as `update`
(matched by `symbol`, `StaticClass.cpp:11767`). Fields per row:

| Field      | Meaning                          |
|------------|----------------------------------|
| `symbol`   | Instrument symbol                |
| `bid`      | Best bid                         |
| `ask`      | Best ask                         |
| `last`     | Last traded price                |
| `ticktime` | Quote timestamp (formatted)      |
| `open`/`high`/`low`/`close` | OHLC (snapshot sends `0`) |

**Stop** — send `UNSUBSCRIBE_TICKER` with the same `subscribeticker` field and
the `tickers` to drop (`Server.cpp:4827`).

### 6.2 Get order data (live order book)

**Request** — type string `FETCH_ORDER_DATA`, carried in the generic fetch
envelope (`fetchclientposition` / `FetchClientPositions`; its `.proto` comment
lists `FETCH_ORDER_DATA` as a valid type):

```
ClientMessage {
    type: "FETCH_ORDER_DATA"
    fetchclientposition: FetchClientPositions {
        type: "FETCH_ORDER_DATA"
    }
}
```

The handler (`Server.cpp:1279`) **immediately** returns a full snapshot via
`SqlData getOrderData` (`SqlData.cpp:44`), then flags the context
(`m_FETCH_ORDER_DATA_FirstTime = 1`) so the order thread keeps pushing live
deltas.

**Response** — `type: "ORDER_DATA"`, field `orderdata` (`OrderData`, number 79).
The snapshot is an `insert` array; live changes arrive as `insert`, `update`
(matched by `order`, `StaticClass.cpp:7097`), and `delete` (`deletekey:["order"]`).
Fields per row:

| Field              | Meaning                              |
|--------------------|--------------------------------------|
| `login`            | Account/login id                     |
| `time`             | Order time                           |
| `deal`             | Deal id                              |
| `order`            | Order id (the update/delete key)     |
| `symbol`           | Instrument symbol                    |
| `type`             | Buy/sell/order type                  |
| `volume`           | Order volume                         |
| `price`            | Order price                          |
| `comment`          | Order comment                        |
| `status`           | Order status                         |
| `select`           | Selection flag                       |
| `statustype`       | Status type                          |
| `subtype`          | Order sub-type                       |
| `contraorder`      | Contra order id                      |
| `tradeexecutetime` | Execution time                       |
| `ourcomment`       | Internal comment                     |
| `orderstate`       | Order state                          |

**Stop** — send `UNSUBSCRIBE` with `requestType:"FETCH_ORDER_DATA"`
(`Server.cpp:1432`).

### 6.3 Get deal data (live deals)

**Request** — type string `FETCH_DEALING_DATA`, carried in the generic fetch
envelope (`fetchDealingdatainterval` / `FetchDealingDataInterval`, field 7),
optionally with a time window:

```
ClientMessage {
    type: "FETCH_DEALING_DATA"
    fetchDealingdatainterval: FetchDealingDataInterval {
        type:           "FETCH_DEALING_DATA"
        subscriptionId: "..."          // optional
        startTime:      1692599400      // optional, unix seconds
        endTime:        1693559242      // optional (currently forced to 0 server-side)
    }
}
```

The handler (`Server.cpp:1295`) registers the client into the deal/order
broadcast lists (`m_ClientList_forDeal` / `m_ClientList_forOrder`) and sets
`m_FETCH_DEALING_DATA_start = 1`. Unlike orders, there is **no immediate
snapshot** — you receive deals live from the moment of subscription onward.

**Response** — `type: "DEALING_DATA"`, field `dealingdata` (`DealingData`,
number 60), one message per deal (`StaticClass.cpp:10678`). Fields:

| Field         | Meaning                                |
|---------------|----------------------------------------|
| `id`          | Deal id                                |
| `time`        | Deal time                              |
| `login`       | Account/login id                       |
| `symbol`      | Instrument symbol                      |
| `buysell`     | Buy / sell side                        |
| `volume`      | Deal volume                            |
| `price`       | Deal price                             |
| `reason`      | Deal reason                            |
| `dealingtype` | Deal type                              |

**Stop** — send `UNSUBSCRIBE` with `requestType:"FETCH_DEALING_DATA"`
(`Server.cpp:1440`); the server removes you from the deal/order broadcast lists.

---

## 7. Choosing which columns a stream sends (active column change)

A stream (net position, orders, …) can carry many columns, but a client usually
displays only a subset. `ACTIVE_COLUMNS_CHANGED` tells the server **which
columns to compute and push** for a given `requestType`, so subsequent stream
frames are trimmed to that set. This is a control message — it changes an
existing subscription; it does not start or stop one.

### Request

Send a `ClientMessage` carrying an `ActiveColumnChanged`
(`ProtoFile/ActiveColumnChanged.proto`, field `activecolumnchagerequest`,
number 5):

```
ClientMessage {
    type: "ACTIVE_COLUMNS_CHANGED"
    activecolumnchagerequest: ActiveColumnChanged {
        type:        "ACTIVE_COLUMNS_CHANGED"
        requestType: "FETCH_CLIENT_POSITIONS"   // which stream these columns apply to
        loginUser:   "1001"
        columns:     ["login","symbol","ag-Grid-AutoColumn","subbroker",
                      "volume","previousvolume","difference"]
    }
}
```

`ActiveColumnChanged` fields: `type=1`, `requestType=2 (string)`,
`columns=3 (repeated string)`, `loginUser=4 (string)`.

### Behaviour

The handler (`Server.cpp:4353`) requires a valid login and both `loginUser` and
`columns` to be present. It rebuilds the per-client column subscription
(`m_clientrequests_List`, keyed `loginUser:column:requestType`): it first
**removes every existing entry** for that `requestType`, then adds one entry per
column in the new list. So each `ACTIVE_COLUMNS_CHANGED` is a full replacement of
the active column set for that `requestType`, not an incremental add — send the
complete desired list every time.

### Response

The server confirms with a `SERVER_MESSAGE`
(`{"type":"SERVER_MESSAGE","responseMessage":"Column Subscription Has Been Updated."}`),
then applies the trimmed column set to subsequent stream frames for that
`requestType`.

> A companion `ACTIVE_COLUMNS_DELETE` message (`Server.cpp:4453`, same
> `activecolumnchagerequest` field) removes columns from the active set instead
> of replacing it.

---

## 8. End-to-end sequence

```
Client                                  OrikaServer
  |  --- wss:// TLS handshake (skip cert verify) --->  |
  |                                                    |
  |  ClientMessage{type:"login", loginrequest{...}} -> |
  |  <- ClientMessage{type:"LOGIN_STATUS",             |
  |        loginresponse{status:"success",             |
  |                      logintoken, data}}            |
  |                                                    |
  |  ClientMessage{type:"FETCH_CLIENT_POSITIONS",      |
  |        fetchclientposition{action:"refresh"}}  ->  |
  |  <- ClientMessage{type:"CLIENT_POSITION",          |
  |        clientposition{insert:[ ...net positions ]}}|
  |  <- ClientMessage{type:"CLIENT_POSITION", ...}  (live updates stream)
  |  ...                                                |
```

---

## 9. Reference client

A working, standalone C++17 reference implementation lives in a **separate**
repo: `D:\VSCodeProject\OrikaClient` (`OrikaClient.sln`, x64, output
`x64\Debug\orikaClient.exe`). It connects over `wss://`, performs the protobuf
login, and reports PASS/FAIL with an exit code. `orikaClient.exe --selftest`
runs offline codec golden-vector checks. It vendors OpenSSL 3.4.0 under
`third_party/openssl/`. Use it as a concrete example of constructing the frames
described above.

---

## 10. Implementation checklist

1. [ ] WebSocket client capable of binary frames + relaxed TLS verification.
2. [ ] Generated protobuf bindings from `ProtoFile/*.proto` (at minimum
       `clientmessage`, `loginrequest`, `loginresponse`, `FetchClientPositions`,
       `ClientpositionInsertandUpdate`/`Clientposition`).
3. [ ] Connect to `wss://<host>:85`.
4. [ ] Send `login` `ClientMessage`; wait for `LOGIN_STATUS`; assert
       `status == "success"`.
5. [ ] Send `FETCH_CLIENT_POSITIONS` `ClientMessage`.
6. [ ] Parse incoming `CLIENT_POSITION` frames; render `insert` rows; apply
       subsequent `insert`/`update` deltas as the stream continues.
7. [ ] (Optional) `UNSUBSCRIBE` `FETCH_CLIENT_POSITIONS` when done.
```
