import json

buf = ""

s = json.load(open("natives.json"))

def filter_reserved_keyword(par):
    if (par == "end"):
        return "_end"
    elif (par == "repeat"):
        return "_repeat"

    return par

for (n, data) in s.items():
    buf += f"{n} = {{}}\n"
    for (hash, entry) in data.items():
        buf += f"{n}.{entry['name']} = function("

        for param in entry["params"]:
            buf += f"{filter_reserved_keyword(param['name'])},"
        buf = buf.removesuffix(",")
        
        invoke_type = "invoke_int"

        if (entry["return_type"] == "void"):
            invoke_type = "invoke_void"
        elif (entry["return_type"] == "float"):
            invoke_type = "invoke_float"
        elif (entry["return_type"] == "BOOL"):
            invoke_type = "invoke_bool"
        elif (entry["return_type"] == "const char*"):
            invoke_type = "invoke_str"
        elif (entry["return_type"] == "Vector3"):
            invoke_type = "invoke_vec3"
        elif (entry["return_type"].endswith("*")):
            invoke_type = "invoke_ptr"

        buf += f") return _invoker.{invoke_type}({hash},"
        for param in entry["params"]:
            buf += f"{filter_reserved_keyword(param['name'])},"
        buf = buf.removesuffix(",")
        buf += ") end\n"
        pass

open("natives.lua", "w+").write(buf)