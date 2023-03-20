TEMPLATE = app
TARGET = firebook
CONFIG = c99 link_pkgconfig
DEFINES = _GNU_SOURCE
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc
PKGCONFIG += gumbo

HEADERS =

SOURCES = \
    0Temp.c \
    main.c \

DISTFILES = \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \


