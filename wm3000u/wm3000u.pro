TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt

INCLUDEPATH	+= .

HEADERS	+=  \
    wm3kscpispecial.h \
    wmglobal.h \
    wmuoffsetcustomlabels.h \
    wm3000u.h \
    pcbiface.h \
    devserver.h \
    confdialogbase.h \
    rangedialogbase.h \
    wmrawactualvalbase.h \
    dbushelper.h \
    dbushelperadapter.h \
    wmviewbaseu.h

SOURCES	+=  \
    main.cpp \
    wm3kscpispecial.cpp \
    wmuoffsetcustomlabels.cpp \
    wm3000u.cpp \
    pcbiface.cpp \
    devserver.cpp \
    wmrawactualvalbase.cpp \
    confdialogbase.cpp \
    rangedialogbase.cpp \
    dbushelper.cpp \
    dbushelperadapter.cpp \
    wmviewbaseu.cpp

FORMS	= rangedialogbase.ui \
	wmrawactualvalbase.ui \
    confdialogbase.ui

IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind \
	images/filenew_1 \
	images/fileopen_1 \
	images/filesave_1 \
	images/print_1 \
	images/undo_1 \
	images/redo_1 \
	images/editcut_1 \
	images/editcopy_1 \
	images/editpaste_1 \
	images/searchfind_1 \
	images/filenew_2 \
	images/fileopen_2 \
	images/filesave_2 \
	images/print_2 \
	images/undo_2 \
	images/redo_2 \
	images/editcut_2 \
	images/editcopy_2 \
	images/editpaste_2 \
	images/searchfind_2 \
	images/filenew_3 \
	images/fileopen_3 \
	images/filesave_3 \
	images/print_3 \
	images/undo_3 \
	images/redo_3 \
	images/editcut_3 \
	images/editcopy_3 \
	images/editpaste_3 \
	images/searchfind_3 \
	images/zeralogo.png \
	images/Flags/ad.png \
	images/Flags/ae.png \
	images/Flags/af.png \
	images/Flags/ag.png \
	images/Flags/ai.png \
	images/Flags/al.png \
	images/Flags/am.png \
	images/Flags/an.png \
	images/Flags/ao.png \
	images/Flags/ar.png \
	images/Flags/as.png \
	images/Flags/at.png \
	images/Flags/au.png \
	images/Flags/aw.png \
	images/Flags/ax.png \
	images/Flags/az.png \
	images/Flags/ba.png \
	images/Flags/bb.png \
	images/Flags/bd.png \
	images/Flags/be.png \
	images/Flags/bf.png \
	images/Flags/bg.png \
	images/Flags/bh.png \
	images/Flags/bi.png \
	images/Flags/bj.png \
	images/Flags/bm.png \
	images/Flags/bn.png \
	images/Flags/bo.png \
	images/Flags/br.png \
	images/Flags/bs.png \
	images/Flags/bt.png \
	images/Flags/bv.png \
	images/Flags/bw.png \
	images/Flags/by.png \
	images/Flags/bz.png \
	images/Flags/ca.png \
	images/Flags/catalonia.png \
	images/Flags/cc.png \
	images/Flags/cd.png \
	images/Flags/cf.png \
	images/Flags/cg.png \
	images/Flags/ch.png \
	images/Flags/ci.png \
	images/Flags/ck.png \
	images/Flags/cl.png \
	images/Flags/cm.png \
	images/Flags/cn.png \
	images/Flags/co.png \
	images/Flags/cr.png \
	images/Flags/cs.png \
	images/Flags/cu.png \
	images/Flags/cv.png \
	images/Flags/cx.png \
	images/Flags/cy.png \
	images/Flags/cz.png \
	images/Flags/de.png \
	images/Flags/dj.png \
	images/Flags/dk.png \
	images/Flags/dm.png \
	images/Flags/do.png \
	images/Flags/dz.png \
	images/Flags/ec.png \
	images/Flags/ee.png \
	images/Flags/eg.png \
	images/Flags/eh.png \
	images/Flags/england.png \
	images/Flags/er.png \
	images/Flags/es.png \
	images/Flags/et.png \
	images/Flags/europeanunion.png \
	images/Flags/fam.png \
	images/Flags/fi.png \
	images/Flags/fj.png \
	images/Flags/fk.png \
	images/Flags/fm.png \
	images/Flags/fo.png \
	images/Flags/fr.png \
	images/Flags/ga.png \
	images/Flags/gb.png \
	images/Flags/gd.png \
	images/Flags/ge.png \
	images/Flags/gf.png \
	images/Flags/gh.png \
	images/Flags/gi.png \
	images/Flags/gl.png \
	images/Flags/gm.png \
	images/Flags/gn.png \
	images/Flags/gp.png \
	images/Flags/gq.png \
	images/Flags/gr.png \
	images/Flags/gs.png \
	images/Flags/gt.png \
	images/Flags/gu.png \
	images/Flags/gw.png \
	images/Flags/gy.png \
	images/Flags/hk.png \
	images/Flags/hm.png \
	images/Flags/hn.png \
	images/Flags/hr.png \
	images/Flags/ht.png \
	images/Flags/hu.png \
	images/Flags/id.png \
	images/Flags/ie.png \
	images/Flags/il.png \
	images/Flags/in.png \
	images/Flags/io.png \
	images/Flags/iq.png \
	images/Flags/ir.png \
	images/Flags/is.png \
	images/Flags/it.png \
	images/Flags/jm.png \
	images/Flags/jo.png \
	images/Flags/jp.png \
	images/Flags/ke.png \
	images/Flags/kg.png \
	images/Flags/kh.png \
	images/Flags/ki.png \
	images/Flags/km.png \
	images/Flags/kn.png \
	images/Flags/kp.png \
	images/Flags/kr.png \
	images/Flags/kw.png \
	images/Flags/ky.png \
	images/Flags/kz.png \
	images/Flags/la.png \
	images/Flags/lb.png \
	images/Flags/lc.png \
	images/Flags/li.png \
	images/Flags/lk.png \
	images/Flags/lr.png \
	images/Flags/ls.png \
	images/Flags/lt.png \
	images/Flags/lu.png \
	images/Flags/lv.png \
	images/Flags/ly.png \
	images/Flags/ma.png \
	images/Flags/mc.png \
	images/Flags/md.png \
	images/Flags/me.png \
	images/Flags/mg.png \
	images/Flags/mh.png \
	images/Flags/mk.png \
	images/Flags/ml.png \
	images/Flags/mm.png \
	images/Flags/mn.png \
	images/Flags/mo.png \
	images/Flags/mp.png \
	images/Flags/mq.png \
	images/Flags/mr.png \
	images/Flags/ms.png \
	images/Flags/mt.png \
	images/Flags/mu.png \
	images/Flags/mv.png \
	images/Flags/mw.png \
	images/Flags/mx.png \
	images/Flags/my.png \
	images/Flags/mz.png \
	images/Flags/na.png \
	images/Flags/nc.png \
	images/Flags/ne.png \
	images/Flags/nf.png \
	images/Flags/ng.png \
	images/Flags/ni.png \
	images/Flags/nl.png \
	images/Flags/no.png \
	images/Flags/np.png \
	images/Flags/nr.png \
	images/Flags/nu.png \
	images/Flags/nz.png \
	images/Flags/om.png \
	images/Flags/pa.png \
	images/Flags/pe.png \
	images/Flags/pf.png \
	images/Flags/pg.png \
	images/Flags/ph.png \
	images/Flags/pk.png \
	images/Flags/pl.png \
	images/Flags/pm.png \
	images/Flags/pn.png \
	images/Flags/pr.png \
	images/Flags/ps.png \
	images/Flags/pt.png \
	images/Flags/pw.png \
	images/Flags/py.png \
	images/Flags/qa.png \
	images/Flags/re.png \
	images/Flags/ro.png \
	images/Flags/rs.png \
	images/Flags/ru.png \
	images/Flags/rw.png \
	images/Flags/sa.png \
	images/Flags/sb.png \
	images/Flags/sc.png \
	images/Flags/scotland.png \
	images/Flags/sd.png \
	images/Flags/se.png \
	images/Flags/sg.png \
	images/Flags/sh.png \
	images/Flags/si.png \
	images/Flags/sj.png \
	images/Flags/sk.png \
	images/Flags/sl.png \
	images/Flags/sm.png \
	images/Flags/sn.png \
	images/Flags/so.png \
	images/Flags/sr.png \
	images/Flags/st.png \
	images/Flags/sv.png \
	images/Flags/sy.png \
	images/Flags/sz.png \
	images/Flags/tc.png \
	images/Flags/td.png \
	images/Flags/tf.png \
	images/Flags/tg.png \
	images/Flags/th.png \
	images/Flags/tj.png \
	images/Flags/tk.png \
	images/Flags/tl.png \
	images/Flags/tm.png \
	images/Flags/tn.png \
	images/Flags/to.png \
	images/Flags/tr.png \
	images/Flags/tt.png \
	images/Flags/tv.png \
	images/Flags/tw.png \
	images/Flags/tz.png \
	images/Flags/ua.png \
	images/Flags/ug.png \
	images/Flags/um.png \
	images/Flags/us.png \
	images/Flags/uy.png \
	images/Flags/uz.png \
	images/Flags/va.png \
	images/Flags/vc.png \
	images/Flags/ve.png \
	images/Flags/vg.png \
	images/Flags/vi.png \
	images/Flags/vn.png \
	images/Flags/vu.png \
	images/Flags/wales.png \
	images/Flags/wf.png \
	images/Flags/ws.png \
	images/Flags/ye.png \
	images/Flags/yt.png \
	images/Flags/za.png \
	images/Flags/zm.png \
	images/Flags/zw.png

######################################################################
# Automatically generated by qmake (1.07a) Thu May 12 09:23:36 2005
######################################################################

TRANSLATIONS    = wm3000u_gb.ts \
                  wm3000u_pl.ts

QT += widgets network xml qt3support dbus core

INCLUDEPATH += \
    ../wm-common \
    ../wm-common/client-access \
    ../wm-common/dsp \
    ../wm-common/gui \
    ../wm-common/just \
    ../wm-common/scales-and-units \
    ../wm-common/scpi/definitions \
    ../wm-common/scpi \
    ../wm-common/settings \
    ../wm-common/service-interfaces

# Qt4.8's QString  on modern env causes loads of warnings
QMAKE_CXXFLAGS += -Wno-deprecated-copy

LIBS += -L../wm-common -lwm-common

target.path = /usr/bin
INSTALLS += target

OTHER_FILES += \
    dbushelperadapter. \
    de.zera.wm3000u.xml


