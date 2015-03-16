import json
import os

def convert_meta(meta_path):
    json_data = open(meta_path)
    meta = json.load(json_data)
    events = meta["events"]

    def convert_action_meta(action_meta):
        li = []
        if action_meta["is_infinite"]:
            repeat = -1
        else:
            repeat = action_meta["times"] - 1
        li.append(str(repeat))
        li.append(str(action_meta["delay"]))
        li.append(str(action_meta["interval"]))
        return ','.join(li)

    def convert_trigger(trigger):
        if trigger["trigger_type"] == "unit_change":
            if "trigger_by_one" in trigger:
                if trigger["trigger_by_one"]:
                    trigger["trigger_count"] = 1
                else:
                    trigger["trigger_count"] = 0
                del trigger["trigger_by_one"]

    for event in events:
        for trigger in event["triggers"]:
            convert_trigger(trigger)
    json_data.close()
    with open(meta_path, "w") as outfile:
        json.dump(meta, outfile, indent=4)
for dir_path, dir_names, file_names in os.walk("."):
    for name in file_names:
        if name == "meta.json":
            convert_meta(os.path.join(dir_path, name))
            break
