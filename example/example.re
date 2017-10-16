open Std;
open Http;

let onError e => {
  switch e {
  | NetworkError => Js.log "NetworkError"
  | Timeout => Js.log "Timeout"
  | BadResponse status msg => Js.log3 "BadResponse" status msg
  };
};

let onSuccess d => {
  Js.log d
};


let query = {|
  {
    authenticate(email: "andreas@sentia.io", password: "password") {
      token
    }
  }
|};
let payload = {
  "query": query,
  "variables": {"session": ""}
};

let body = Option.withDefault "" (Js.Json.stringifyAny payload);

Http.post ::body "https://gain.ai:8090/graphql"
  |> Task.fork onError onSuccess;
   