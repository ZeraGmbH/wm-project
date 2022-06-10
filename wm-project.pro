TEMPLATE = subdirs

SUBDIRS = \
    unittests \
    wm-common \
    wm3000i \
    wm3000u

unittests.depends = wm-common
wm3000i.depends = wm-common
wm3000u.depends = wm-common

