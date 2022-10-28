import os
# assign directory
directory = '/mnt/c/Program Files (x86)/Steam/steamapps/common/Victoria 3/game/map_data/state_regions'

print('0.0.0 = {')

for filename in os.listdir(directory):
  filename = os.path.join(directory, filename)
  if os.path.isfile(filename):
    file_string = os.path.splitext(os.path.basename(filename))[0][3:]
    print('\tlink = { comment = "* **********" }')
    print('\tlink = { comment = ' + file_string + ' }')
    print('\tlink = { comment = "* **********" }')
    print('\tlink = { comment = "* " }')

    file = open(filename, encoding='utf-8-sig')
    bracket_level = 0
    for line in file:
      if not line:
        continue
      if line == '\n':
        continue
      if bracket_level == 0:
        print('\tlink = { comment = "* * ' + line[:-4] + '" }')
      if '{' in line:
        bracket_level += 1
      if '}' in line:
        bracket_level -= 1

    print('\tlink = { comment = "* " }')
print('}')
