open Http;
open Js.Result;

let callback = (res) => switch res {
| Error(e) => switch e {
  | NetworkError => Js.log("NetworkError")
  | Timeout => Js.log("Timeout")
  | BadResponse(status, msg) => Js.log3("BadResponse", status, msg)
  }
| Ok(d) => Js.log(d)
};
let query = {|
  {
    authenticate(email: "andreas@sentia.io", password: "password") {
      token
    }
  }
|};

let payload = {"query": query, "variables": {"session": ""}};

let body = switch (Js.Json.stringifyAny(payload)) {
| None => ""
| Some(body) => body
};

Http.post(~body, "https://gain.ai:8090/graphql", callback) 
