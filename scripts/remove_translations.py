# Usage `python3 remove_translations.py {path_to_file}`

import json, sys, urllib.request

with urllib.request.urlopen("https://raw.githubusercontent.com/YimMenu/Translations/master/en_US.json") as url:
    translation_data = json.load(url)

with open(sys.argv[1]) as file:
    file_string = file.read()
    for key in translation_data:
        file_string = file_string.replace(f'"{key}"', f'"{translation_data[key]}"')

    file_string = file_string.replace('"_T.data()', '"')
    file_string = file_string.replace('"_T', '"')
    file.close()

with open(sys.argv[1], 'w') as file:
    file.write(file_string)
    file.close()
    print("Done!")

