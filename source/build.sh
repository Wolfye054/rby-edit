#!/bin/bash
set -e

CFLAGS="-g -Wall -Wextra -Werror -Wno-discarded-qualifiers -Wno-unused-parameter"
GTK_FLAGS=$(pkg-config --cflags --libs gtk4)
RESOURCE_XML="../build/resources.xml"

mkdir -p ../build

# generate resources.xml
cat > "$RESOURCE_XML" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
    <gresource prefix="/rbyedit">
EOF

find ../assets/item ../assets/pokemon -type f | sort | while read -r file; do
    # Remove the ../ prefix
    resource="${file#../}"
    echo "        <file>$resource</file>" >> "$RESOURCE_XML"
done

cat >> "$RESOURCE_XML" <<EOF
    </gresource>
</gresources>
EOF

# compile the resource
glib-compile-resources \
    --target=../build/resources.c \
    --generate-source \
    --sourcedir=.. \
	"$RESOURCE_XML"

# build
pushd ../build >> /dev/null
gcc ../source/gtk_rbyedit.c \
	../source/rbyedit.c \
	../source/rbyinfo.c \
	resources.c \
	$CFLAGS $GTK_FLAGS \
	-o rbyedit
cp rbyedit debug
popd >> /dev/null
