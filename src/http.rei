open Std;
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



let get : headers::(dict string string)? => string => task error response;
let post : headers::(dict string string)? => body::string => string => task error response;
let put : headers::(dict string string)? => body::string => string => task error response;
let delete : headers::(dict string string)? => string => task error response;
