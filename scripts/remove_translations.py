# Usage `python3 remove_translations.py {path_to_file}`

import json, sys, glob, urllib.request, os, argparse

parser = argparse.ArgumentParser(prog='Remove translations', description='Removes translations from YimMenu source files and replaces it with en_US.json')

parser.add_argument('target', type=str, help='File to remove translations from, accepts glob patterns')
parser.add_argument('--use_local', action='store_true', help='Use local translations json')
parser.add_argument('--translation_file', type=str, default="en_US.json", help='Name of the local translation file')

args = parser.parse_args()

if args.use_local:
    with open(args.translation_file, 'r') as translation_file_content:
        translation_data = json.load(translation_file_content)
else:
    with urllib.request.urlopen('https://raw.githubusercontent.com/YimMenu/Translations/master/en_US.json') as url:
        translation_data = json.load(url)

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

for arg_file in glob.iglob(args.target, recursive=True):
    if not os.path.isdir(arg_file):
        print(arg_file)
        remove_from_file(arg_file)
