open Belt;

[@bs.send]
external _splitString : (string, string) => array(string) = "split";

let splitString = (t, separator) =>
  _splitString(t, separator) |. List.fromArray;

module Xhr = {
  type t;
  [@bs.new] external make : unit => t = "XMLHttpRequest";
  [@bs.set]
  external onReady : (t, unit => unit) => unit = "onreadystatechange";
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

let parseHeaders = headers =>
  headers
  |. splitString("\n")
  |. List.reduce([], (acc, s) =>
       switch (splitString(":", s)) {
       | [k, v] => [(String.trim(k), String.trim(v)), ...acc]
       | _ => acc
       }
     );

type response = {
  status: int,
  statusText: string,
  headers: list((string, string)),
  body: string,
};

type error =
  | Timeout
  | NetworkError
  | BadResponse(response);

let rec setDefault = (key, value, lst) =>
  switch (lst) {
  | [] => []
  | [(k, _), ..._] when key == k => lst
  | [x, ...xs] => [x, ...setDefault(key, value, xs)]
  };

let make = (~method as pub_, ~headers as userHeaders, ~body=?, url, callback) => {
  open Js.Result;
  let fail = x => callback(Error(x));
  let succeed = x => callback(Ok(x));
  let request = Xhr.make();
  Xhr.open_(request, pub_, url);
  let headers = setDefault("Content-Type", "application/json", userHeaders);
  List.forEach(headers, ((v, k)) => Xhr.setRequestHeader(request, k, v));
  let onReady = () => {
    let readyState = Xhr.readyState(request);
    if (readyState == 4) {
      let statusText = Xhr.statusText(request);
      let responseBody = Xhr.responseText(request);
      let responseHeaders =
        Xhr.getAllResponseHeaders(request) |> parseHeaders;
      switch (Xhr.status(request)) {
      | 0 => fail(NetworkError)
      | status when status < 200 || status >= 300 =>
        fail @@
        BadResponse({
          status,
          statusText,
          headers: responseHeaders,
          body: responseBody,
        })
      | status =>
        succeed({
          status,
          statusText,
          headers: responseHeaders,
          body: responseBody,
        })
      };
    };
  };
  Xhr.onReady(request, onReady);
  Xhr.onTimeout(request, (_) => fail(Timeout));
  Xhr.send(request, Js.Null.fromOption(body));
};

let get = (~headers=[], url) => make(~method="GET", ~headers, url);

let post = (~headers=[], ~body, url) =>
  make(~method="POST", ~headers, ~body, url);

let put = (~headers=[], ~body, url) =>
  make(~method="PUT", ~headers, ~body, url);

let delete = (~headers=[], url) => make(~method="DELETE", ~headers, url);