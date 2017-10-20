open Std;
module Xhr = {
  type t;

  external make: unit => t = "XMLHttpRequest" [@@bs.new];
  external onReady : t => (unit => unit) => unit = "onreadystatechange" [@@bs.set];
  external onTimeout : t => (unit => unit) => unit = "ontimeout" [@@bs.set];
  external setRequestHeader : t => string => string => unit = "" [@@bs.send];
  external open_ : t => string => string => unit = "open" [@@bs.send];
  external send : t => Js.null string => unit = "" [@@bs.send];
  external readyState : t => int = "" [@@bs.get];
  external status : t => int = "" [@@bs.get];
  external statusText : t => string = "" [@@bs.get];
  external responseText : t => string = "" [@@bs.get];
  external getAllResponseHeaders : t => string = "" [@@bs.send];
};

let parseHeaders headers => 
  headers
    |> String.split "\n"
    |> List.filterMap (fun s => switch (String.split ":" s) {
      | [k, v] => Some (String.trim k, String.trim v)
      | _ => None
    })
    |> Dict.fromList;


type response = {
  status: int,
  statusText: string,
  headers: dict string string,
  body: string
};


type error = 
  | Timeout
  | NetworkError
  | BadResponse int string;


let make ::method headers::userHeaders ::body=? url =>
  Task.make (fun fail succeed => {
    let request = Xhr.make ();
    Xhr.open_ request method url;

    let headers = Dict.setDefault "Content-Type" "application/json" userHeaders;
    Dict.iter (Xhr.setRequestHeader request) headers;

    let onReady () => {
      let readyState = Xhr.readyState request;
      if (readyState == 4) {
        let statusText = Xhr.statusText request;
        let responseText = Xhr.responseText request;
        let responseHeaders = Xhr.getAllResponseHeaders request |> parseHeaders;
        switch (Xhr.status request) {
          | 0 => fail (NetworkError)
          | status when status < 200 => fail @@ BadResponse status responseText
          | status when status >= 300 => fail @@ BadResponse status responseText
          | status => {
            succeed {
              status,
              statusText,
              headers: responseHeaders,
              body: responseText
            };
          }
        };
      };
    };

    Xhr.onReady request onReady;
    Xhr.onTimeout request (fun _ => fail Timeout);

    Xhr.send request (Js.Null.from_opt body);

  });
let get ::headers=Dict.empty url =>
  make method::"GET" ::headers url;

let post ::headers=Dict.empty ::body url =>
  make method::"POST" ::headers ::body url;

let put ::headers=Dict.empty ::body url =>
  make method::"PUT" ::headers ::body url;

let delete ::headers=Dict.empty url =>
  make method::"DELETE" ::headers url;