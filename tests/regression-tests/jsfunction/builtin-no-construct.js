/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

function checkMethod(method) {
    try {
        new method();
        print("not reached " + method);
    } catch (e) {
        print("method is not a constructor");
    }
}

function checkMethods(proto) {
    var names = Object.getOwnPropertyNames(proto);
    for (var i = 0; i < names.length; i++) {
        var name = names[i];
        if (name == "constructor")
            continue;
        var prop = proto[name];
        if (typeof prop === "function")
            checkMethod(prop);
    }
}

checkMethod(Function.prototype);
checkMethods(JSON);
