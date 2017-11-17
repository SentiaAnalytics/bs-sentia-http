let take = (i, str) => String.sub(str, 0, i);

let drop = (i, str) => String.sub(str, i, String.length(str));

let splitString = (separator, str) => {
  let len = String.length(separator);
  let rec go = (i, str) => {
    let sub = String.sub(str, i, len);
    switch () {
    | () when sub == separator => [take(i, str), ...go(0, drop(i + len, str))]
    | () when i == String.length(str) => [str]
    | () => go(i + 1, str)
    }
  };
  go(0, str)
};


module Xhr = {
  type t;
  [@bs.new] external make : unit => t = "XMLHttpRequest";
  [@bs.set] external onReady : (t, unit => unit) => unit = "onreadystatechange";
  [@bs.set] external onTimeout : (t, unit => unit) => unit = "ontimeout";
  [@bs.send] external setRequestHeader : (t, string, string) => unit = "";
  [@bs.send] external open_ : (t, string, string) => unit = "open";
  [@bs.send] external send : (t, Js.null(string)) => unit = "";
  [@bs.get] external readyState : t => int = "";
  [@bs.get] external status : t => int = "";
  [@bs.get] external statusText : t => string = "";
  [@bs.get] external responseText : t => string = "";
  [@bs.send] external getAllResponseHeaders : t => string = "";
};

let parseHeaders = (headers) =>
  headers
  |> splitString("\n")
  |> List.fold_left( 
       (acc, s) =>
         switch (splitString(":", s)) {
         | [k, v] => [((String.trim(k), String.trim(v))), ...acc]
         | _ => acc
         }
    ,[]);

type response = {
  status: int,
  statusText: string,
  headers: list((string, string)),
  body: string
};

type error =
  | Timeout
  | NetworkError
  | BadResponse(int, string);

let rec setDefault = (key, value, lst) => switch lst {
  | [] => []
  | [(k, v), ...xs] when key == k => lst
  | [x, ...xs] => [x, ...setDefault(key, value, xs)]
};

let make = (~method as pub_, ~headers as userHeaders, ~body=?, url, callback) => {
  open Js.Result;

  let fail = (x) => callback(Error(x));
  let succeed = (x) => callback(Ok(x));

  let request = Xhr.make();
  Xhr.open_(request, pub_, url);
  let headers = setDefault("Content-Type", "application/json", userHeaders);
  List.iter(((k, v)) => Xhr.setRequestHeader(request, k, v), headers);
  let onReady = () => {
    let readyState = Xhr.readyState(request);
    if (readyState == 4) {
      let statusText = Xhr.statusText(request);
      let responseText = Xhr.responseText(request);
      let responseHeaders = Xhr.getAllResponseHeaders(request) |> parseHeaders;
      switch (Xhr.status(request)) {
      | 0 => fail(NetworkError)
      | status when status < 200 => fail @@ BadResponse(status, responseText)
      | status when status >= 300 => fail @@ BadResponse(status, responseText)
      | status => succeed({status, statusText, headers: responseHeaders, body: responseText})
      }
    }
  };
  Xhr.onReady(request, onReady);
  Xhr.onTimeout(request, (_) => fail(Timeout));
  Xhr.send(request, Js.Null.from_opt(body))
};

let get = (~headers=[], url) => make(~method="GET", ~headers, url);

let post = (~headers=[], ~body, url) => make(~method="POST", ~headers, ~body, url);

let put = (~headers=[], ~body, url) => make(~method="PUT", ~headers, ~body, url);

let delete = (~headers=[], url) => make(~method="DELETE", ~headers, url);
