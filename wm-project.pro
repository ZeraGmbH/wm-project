TEMPLATE = subdirs

SUBDIRS = \
    tests \
    wm-common \
    wm3000i \
    wm3000u

tests.depends = wm-common
wm3000i.depends = wm-common
wm3000u.depends = wm-common

