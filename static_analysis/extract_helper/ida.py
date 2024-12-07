import idautils, ida_funcs
import os
import json

maybe_critical_funcs = {
    "memcpy": [3],
    "memncpy": [3],
    "memmove": [3],
    "snprintf": [2, 3],
    "vsnprintf": [2, 3],
    "sprintf": [2, 3, 4, 5, 6, 7, 8],
    "vsprintf": [2, 3, 4, 5, 6, 7, 8],
    "sscanf": [1, 2],
    "strcat": [2],
    "strcpy": [2],
    "strncat": [3],
    "strncpy": [3],
    "spliter": [1, 2],
    "bcopy": [3],
    "strscat": [1, 2],
    "getenv": [0],
    "setenv": [2],
    "nvram_set": [2],
    "nvram_get": [0],
    "Packt_WebGetsVar": [0],
    "jsonObjectGetString": [0],
    "jsonGetObjectString": [0],
    "json_object_get_decode": [0],
    "recv_http_response": [2],
    "os_file_get": [1],
    "os_get_file": [1],
    "recvfrom": [2, 0],
    "recv": [2, 0],
}


def get_named_func():
    named_funcs = []
    for ea in idautils.Functions():
        name = ida_funcs.get_func_name(ea)
        if not name.startswith("sub_") and not name.startswith("nullsub_"):
            criticalIndex = [-1]
            for maybe_critical_func in maybe_critical_funcs:
                if maybe_critical_func in name:
                    criticalIndex = maybe_critical_funcs[maybe_critical_func]
            named_funcs.append(
                {
                    "funcName": name,
                    "offset": hex(ea).strip("L"),
                    "criticalIndex": str(criticalIndex),
                }
            )
    return named_funcs


from pathlib import Path

os.makedirs("extrace_result", exist_ok=True)
in_file = Path(get_input_file_path())
cwd = Path(os.getcwd())

with (cwd / "extrace_result" / (in_file.name + ".simresult")).open("w") as f:
    json.dump(get_named_func(), f, indent=2)
    print(
        f"IDA extraction result has been saved in {(cwd / 'extrace_result' / (in_file.name + '.simresult')).as_posix()}"
    )
