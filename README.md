simple http library for reason/ocaml


#example

```
let getUser =Http.get("https://api.example.com/user");


getUser @@ fun
  | Error(TimeOut) => Js.log("TIMEOUT")
  | Error(NetworkError) => 
    Js.log("NETWORK ERROR. Are you connected to the internet?")
  | Error(BadResponse(response)) => handleError(statusCode, body)
  | Ok(response) => userFetched(response.body |> parseUser);
```