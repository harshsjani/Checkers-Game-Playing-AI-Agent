from collections import defaultdict
import os
import subprocess


name_dicts = defaultdict(list)

def read_files(files):
    score = 0
    for file in files:
        with open(r"/mnt/c/Users/hearm/downloads" + "/" + file) as f:
            score += int(f.read())
    return score

for root, dir, files in os.walk(r"/mnt/c/Users/hearm/downloads"):
    total_files = len(files)
    for i, file in enumerate(files):
        print("File {}/{}".format(i, total_files))
        if "hw3_" in file:#.endswith("_1.txt"):
            lidx = file.index("hw3_") + 4
            ridx = file.rindex("_")

            name = file[lidx:ridx]
            name_dicts[name].append(file)

name_scores = {}
for k in name_dicts:
    name_scores[k] = 0

for name, files in name_dicts.items():
    print("Compiling for {}".format(name))
    name_scores[name] = read_files(files)

scores = list(name_scores.items())
scores.sort(reverse=True, key=lambda x: x[1])

for k, v in scores:
    print(k, v)

with open(r"/mnt/c/Users/hearm/downloads/_compiled_scores.txt", "w") as f:
    f.writelines("\n".join(list(map(str, scores))))
