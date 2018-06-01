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

let get:
  (
    ~headers: list((string, string))=?,
    string,
    Belt.Result.t(response, error) => unit
  ) =>
  unit;

let post:
  (
    ~headers: list((string, string))=?,
    ~body: string,
    string,
    Belt.Result.t(response, error) => unit
  ) =>
  unit;

let put:
  (
    ~headers: list((string, string))=?,
    ~body: string,
    string,
    Belt.Result.t(response, error) => unit
  ) =>
  unit;

let delete:
  (
    ~headers: list((string, string))=?,
    string,
    Belt.Result.t(response, error) => unit
  ) =>
  unit;