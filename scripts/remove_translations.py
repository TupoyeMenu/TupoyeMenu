# Usage `python3 remove_translations.py {path_to_file}`

import json, sys, glob, urllib.request, os

#with urllib.request.urlopen("https://raw.githubusercontent.com/YimMenu/Translations/master/en_US.json") as url:
#    translation_data = json.load(url)

with open('en_US.json', 'r') as translation_file:
    translation_data = json.load(translation_file)

def remove_from_file(file_to_edit):
    with open(file_to_edit) as file:
        file_string = file.read()
        for key in translation_data:
            file_string = file_string.replace(f'"{key}"', f'"{translation_data[key]}"')

        file_string = file_string.replace('"_T.data()', '"')
        file_string = file_string.replace('"_T', '"')
        file.close()

    with open(file_to_edit, 'w') as file:
        file.write(file_string)
        file.close()
        print(f"Done removing translations in: {file_to_edit}!")

for arg_file in glob.iglob(sys.argv[1], recursive=True):
    if not os.path.isdir(arg_file):
        print(arg_file)
        remove_from_file(arg_file)
