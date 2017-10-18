// Generated by BUCKLESCRIPT VERSION 1.9.3, PLEASE EDIT WITH CARE
'use strict';

var Std     = require("bs-sentia-std/lib/js/src/Std.js");
var Curry   = require("bs-platform/lib/js/curry.js");
var Js_null = require("bs-platform/lib/js/js_null.js");

function parseHeaders(headers) {
  return Std.Dict[/* fromList */1](Std.List[/* filterMap */27]((function (s) {
                    var match = Std.$$String[/* split */23](":", s);
                    if (match) {
                      var match$1 = match[1];
                      if (match$1 && !match$1[1]) {
                        return /* Some */[/* tuple */[
                                  Std.$$String[/* trim */14](match[0]),
                                  Std.$$String[/* trim */14](match$1[0])
                                ]];
                      } else {
                        return /* None */0;
                      }
                    } else {
                      return /* None */0;
                    }
                  }), Std.$$String[/* split */23]("\n", headers)));
}

function make(pub, userHeaders, body, url) {
  var partial_arg = Std.Task[/* make */0];
  return (function (param, param$1) {
      return partial_arg((function (fail, succeed) {
                    var request = new XMLHttpRequest();
                    request.open(pub, url);
                    var headers = Std.Dict[/* setDefault */6]("Content-Type", "application/json", userHeaders);
                    Std.Dict[/* iter */9]((function (param, param$1) {
                              request.setRequestHeader(param, param$1);
                              return /* () */0;
                            }))(headers);
                    var onReady = function () {
                      var readyState = request.readyState;
                      if (readyState === 4) {
                        var statusText = request.statusText;
                        var responseText = request.responseText;
                        var responseHeaders = parseHeaders(request.getAllResponseHeaders());
                        console.log("Headers", responseHeaders);
                        var status = request.status;
                        if (status !== 0) {
                          if (status < 200) {
                            return Curry._1(fail, /* BadResponse */[
                                        status,
                                        responseText
                                      ]);
                          } else if (status >= 300) {
                            return Curry._1(fail, /* BadResponse */[
                                        status,
                                        responseText
                                      ]);
                          } else {
                            return Curry._1(succeed, /* record */[
                                        /* status */status,
                                        /* statusText */statusText,
                                        /* headers */responseHeaders,
                                        /* body */responseText
                                      ]);
                          }
                        } else {
                          return Curry._1(fail, /* NetworkError */1);
                        }
                      } else {
                        return 0;
                      }
                    };
                    request.onreadystatechange = onReady;
                    request.ontimeout = (function () {
                        return Curry._1(fail, /* Timeout */0);
                      });
                    request.send(Js_null.from_opt(body));
                    return /* () */0;
                  }), param, param$1);
    });
}

function get($staropt$star, url) {
  var headers = $staropt$star ? $staropt$star[0] : Std.Dict[/* empty */0];
  return make("GET", headers, /* None */0, url);
}

function post($staropt$star, body, url) {
  var headers = $staropt$star ? $staropt$star[0] : Std.Dict[/* empty */0];
  return make("POST", headers, /* Some */[body], url);
}

function put($staropt$star, body, url) {
  var headers = $staropt$star ? $staropt$star[0] : Std.Dict[/* empty */0];
  return make("PUT", headers, /* Some */[body], url);
}

function $$delete($staropt$star, url) {
  var headers = $staropt$star ? $staropt$star[0] : Std.Dict[/* empty */0];
  return make("DELETE", headers, /* None */0, url);
}

exports.get      = get;
exports.post     = post;
exports.put      = put;
exports.$$delete = $$delete;
/* No side effect */