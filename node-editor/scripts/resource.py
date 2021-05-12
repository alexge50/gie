import sys
import os

if len(sys.argv) == 2:
    sys.exit()


def build_resource(file):
    _, filename = os.path.split(file)
    filename = filename.replace('.', '_').replace(' ', '_')

    with open(file, 'rb') as f:
        content = bytearray(f.read())
    content = [hex(x) for x in content]
    content.append('0x00')
    content = ', '.join(content)

    return filename, content


files = sys.argv[2::]
output_file = sys.argv[1]
resources = [build_resource(f) for f in files]
output = '\n'.join(
    f'inline unsigned char {name}[] = {{ {content} }};' for name, content in resources
)

with open(output_file, 'w') as f:
    f.write(output)
