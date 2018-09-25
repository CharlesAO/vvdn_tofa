#!/bin/bash

RTEMS_BSP="ma2100 ma2x5x ma2x8x"
MV_RTEMS_ARCH="myriad2-sparc-rtems"
CURRENT_DIR="$PWD"
TOOLS_INSTALL_DIR="$PWD/StagingArea/tools"
RTEMS_INSTALL_DIR="$PWD/StagingArea/output"
RTEMS_BUILD_DIR="$PWD/StagingArea/RTEMS/Build"
RTEMS_TESTS_DIR="$PWD/StagingArea/RTEMS/Tests"
REPOSITORIES_DIR="$PWD/StagingArea/repos"
LOGGING_DIR="$PWD/StagingArea/logs"
EXTRA_OPT=

function find_tools {
    #Get path of gcc toolchain
    if [ -z "${MV_TOOLS_DIR}" ]; then
        MV_TOOLS_DIR=${HOME}/WORK/Tools/Releases/General
    fi

    if [ -z "${MV_TOOLS_VERSION}" ]; then
        MV_TOOLS_VERSION=`cat ../../toolssettings.mk |\
            grep "^MV_TOOLS_VERSION" |\
            awk '{print $3}' |\
            grep -v "Latest"`
    fi

    tools_base="${MV_TOOLS_DIR}/${MV_TOOLS_VERSION}"

    if [ ! -d ${tools_base} ]; then
        echo "Tools do not exist at ${tools_base}";
        exit 1;
    fi

    platform='unknown'
    unamestr=`uname`
    if [[ "$unamestr" == 'Linux' ]]; then
        platform='linux64'
    else
        platform='win32'
    fi

    MV_GCC_TOOLS=`cat $tools_base/common/toolspaths.mk|grep MV_GCC_TOOLS|head -n 1|awk -F= '{print $2}'`
    gcc_version=`cat $tools_base/common/toolspaths.mk|grep GCCVERSION|head -n 1|awk -F= '{print $2}'`
    gcc_dir=$MV_GCC_TOOLS-$gcc_version

    MV_GCC_PATH=${tools_base}/$platform/$gcc_dir
}

function create_links {
    for i in `ls ${MV_GCC_PATH}/bin`; do
        j=`echo $i|sed -r "s/${MV_GCC_TOOLS}/${MV_RTEMS_ARCH}/"`
        ln -sf ${MV_GCC_PATH}/bin/$i ${TOOLS_INSTALL_DIR}/bin/$j
    done;
}

#Compile auto-tools
function build_autotools {
    pushd ${REPOSITORIES_DIR}
    if [ ! -d MovidiusRTEMS-source-builder ]; then
        git clone git@github.com:movidius/MovidiusRTEMS-source-builder.git
    fi
    popd

    pushd ${REPOSITORIES_DIR}/MovidiusRTEMS-source-builder/rtems &&
        ../source-builder/sb-check > /dev/null &&
        ../source-builder/sb-set-builder --prefix=${TOOLS_INSTALL_DIR} 4.12/rtems-autotools 2>&1 | tee ${LOGGING_DIR}/autotools.log
    if [ ${PIPESTATUS[0]} -eq 1 ]; then
        echo "Autotools failed to setup";
        exit 1;
    fi

    echo "*******************************"
    echo "* Autotools compiled properly *"
    echo "*******************************"
    popd
}

#Compile rtems-tools
function build_rtemstools {
    pushd ${REPOSITORIES_DIR}
    if [ ! -d MovidiusRTEMS-tools ]; then
        git clone git@github.com:movidius/MovidiusRTEMS-tools.git
    fi
    popd

    pushd ${REPOSITORIES_DIR}/MovidiusRTEMS-tools &&
        ./waf distclean | tee ${LOGGING_DIR}/rtemstools.log &&
        ./waf configure --prefix=${TOOLS_INSTALL_DIR} | tee -a ${LOGGING_DIR}/rtemstools.log &&
        ./waf build install | tee -a ${LOGGING_DIR}/rtemstools.log &&
        ./waf distclean | tee -a ${LOGGING_DIR}/rtemstools.log
    if [ ${PIPESTATUS[0]} -eq 1 ]; then
        echo "RTEMS tools failed to setup";
        exit 1;
    fi

    echo "*********************************"
    echo "* RTEMS tools compiled properly *"
    echo "*********************************"
    popd
}

function reduce_rtems_size {
    libbsp_arch=""
    libbsp_path="c/src/lib/libbsp"
    for i in `ls ${libbsp_path}`;do
        if [ -d ${libbsp_path}/$i ]; then
            if [ "$i" != "myriad2" ] && [ "$i" != "shared" ]; then
                libbsp_arch=${libbsp_arch}" $i";
            fi
        fi;
    done
    for i in ${libbsp_arch}; do
        rm -rf ${libbsp_path}/$i;
    done

    libcpu_arch=""
    libcpu_path="c/src/lib/libcpu"
    for i in `ls ${libcpu_path}`;do
        if [ -d ${libcpu_path}/$i ]; then
            if [ "$i" != "myriad2" ] && [ "$i" != "shared" ]; then
                libcpu_arch=${libcpu_arch}" $i";
            fi
        fi;
    done
    for i in ${libcpu_arch}; do
        rm -rf ${libcpu_path}/$i;
    done

    # We cannot clean cpukit/score/cpu folder because central cpukit configure file needs them
    # cpukit_arch=""
    # cpukit_path="cpukit/score/cpu"
    # for i in `ls ${cpukit_path}`;do
    #     if [ -d ${cpukit_path}/$i ]; then
    #         if [ "$i" != "myriad2" ] && [ "$i" != "shared" ]; then
    #             cpukit_arch=${cpukit_arch}" $i";
    #         fi
    #     fi;
    # done
    # for i in ${cpukit_arch}; do
    #     rm -rf ${cpukit_path}/$i;
    # done
}

#Compile RTEMS
function build_rtems {
    pushd ${REPOSITORIES_DIR}
    if [ ! -d MovidiusRTEMS ]; then
        git clone git@github.com:movidius/MovidiusRTEMS.git
    fi
    popd

    pushd ${REPOSITORIES_DIR}/MovidiusRTEMS

    if [ ! -f cpukit/Makefile.in ]; then

        if [ -d c/src/lib/libbsp/sparc ]; then
            reduce_rtems_size;
        fi

        ./bootstrap -c;
        ./bootstrap -p;
        ./bootstrap;
    fi
    popd

    for i in $RTEMS_BSP; do
        if [ ! -d ${RTEMS_BUILD_DIR}/$i ]; then
            mkdir -p ${RTEMS_BUILD_DIR}/$i
            pushd ${RTEMS_BUILD_DIR}/$i
            param="--prefix=${RTEMS_INSTALL_DIR} --target=${MV_RTEMS_ARCH} --enable-drvmgr=no --enable-posix --enable-cxx --disable-tests"
            if [ "$i" == "ma2x8x" ]; then
                param="$param --disable-networking $EXTRA_OPT"
            else
                param="$param --enable-networking  $EXTRA_OPT"
            fi
            ${REPOSITORIES_DIR}/MovidiusRTEMS/configure ${param} ||\
                { echo "RTEMS configure failed"; exit 1; }
        else
            pushd ${RTEMS_BUILD_DIR}/$i
        fi
        make all install RTEMS_BSP=$i ||\
            { echo "RTEMS for $i failed to setup"; exit 1; }
    done

    echo "***************************"
    echo "* RTEMS compiled properly *"
    echo "***************************"
}

#Compile RTEMS-libbsd
function build_rtems_libbsd {
    pushd ${REPOSITORIES_DIR}
    if [ ! -d MovidiusRTEMS-libbsd ]; then
        git clone git@github.com:movidius/MovidiusRTEMS-libbsd.git
        pushd MovidiusRTEMS-libbsd
        git submodule init
        git submodule update rtems_waf
        popd
    fi
    popd

    tools_waf=${REPOSITORIES_DIR}/MovidiusRTEMS-tools/waf
    if [ ! -e $tools_waf ]; then
        echo "Couldn't find tools waf binary."
        exit 1
    fi

    # Remove last element of arch tuple.
    # eg. myriad2-sparc-rtems -> myriad2-sparc
    libbsd_arch=${MV_RTEMS_ARCH%-*}

    for i in $RTEMS_BSP; do
        if [ "$i" == "ma2x8x" ]; then
            pushd ${REPOSITORIES_DIR}/MovidiusRTEMS-libbsd

            $tools_waf distclean | tee ${LOGGING_DIR}/rtems_libbsd.log
            if [ ${PIPESTATUS[0]} -eq 1 ]; then
                echo "RTEMS libbsd distclean failed";
                exit 1;
            fi

            $tools_waf configure --prefix=${RTEMS_INSTALL_DIR} \
                --rtems-tools=${TOOLS_INSTALL_DIR} \
                --rtems-bsps=${libbsd_arch}/${i} \
                --rtems-version="" | tee -a ${LOGGING_DIR}/rtems_libbsd.log
            if [ ${PIPESTATUS[0]} -eq 1 ]; then
                echo "RTEMS libbsd configure failed";
                exit 1;
            fi

            $tools_waf build install | tee -a ${LOGGING_DIR}/rtems_libbsd.log
            if [ ${PIPESTATUS[0]} -eq 1 ]; then
                echo "RTEMS libbsd build failed";
                exit 1;
            fi

            $tools_waf distclean | tee -a ${LOGGING_DIR}/rtems_libbsd.log

            popd
        fi
    done

    echo "*********************************"
    echo "* RTEMS libbsd compiled properly *"
    echo "*********************************"
}

#Build RTEMS tests
function build_rtems_tests {
    pushd ${REPOSITORIES_DIR}
    if [ ! -d MovidiusRTEMS ]; then
        echo "RTEMS repo doesn't exist";
        exit 1;
    fi
    popd

    pushd ${REPOSITORIES_DIR}/MovidiusRTEMS

    if [ ! -f cpukit/Makefile.in ]; then
        echo "RTEMS repo isn't bootstrapped";
        exit 1;
    fi
    popd

    for i in $RTEMS_BSP; do
        if [ ! -d ${RTEMS_BUILD_DIR}/$i ]; then
            echo "RTEMS build folder doesn't exist for $i";
            exit 1;
        fi
    done

    for i in $RTEMS_BSP; do
        if [ ! -d ${RTEMS_TESTS_DIR}/$i ]; then
            mkdir -p ${RTEMS_TESTS_DIR}/$i
            pushd ${RTEMS_TESTS_DIR}/$i
            export CFLAGS=`cat ${RTEMS_BUILD_DIR}/$i/${MV_RTEMS_ARCH}/c/$i/make/target.cfg | grep ^CFLAGS |cut -d "=" -f 2-`
            param="--enable-drvmgr=no --enable-posix --enable-cxx --enable-tests --cache-file=/dev/null \
                --host=${MV_RTEMS_ARCH} --target=${MV_RTEMS_ARCH} RTEMS_BSP=$i                          \
                --enable-rtems-root=${RTEMS_BUILD_DIR}/$i/${MV_RTEMS_ARCH}/c/$i/                        \
                --enable-project-root=${RTEMS_BUILD_DIR}/$i/${MV_RTEMS_ARCH}/$i/                        \
                --with-project-top=${RTEMS_BUILD_DIR}/$i/${MV_RTEMS_ARCH}/c/"
            if [ "$i" == "ma2x8x" ]; then
                param="--disable-networking $param"
            else
                param="--enable-networking $param"
            fi
	    param="$param $EXTRA_OPT"
            ${REPOSITORIES_DIR}/MovidiusRTEMS/testsuites/configure ${param}

        else
            pushd ${RTEMS_TESTS_DIR}/$i
        fi
        make all RTEMS_BSP=$i
        if [ $? -eq 1 ]; then
            echo "RTEMS tests for $i failed to setup";
            exit 1;
        fi

        unset CFLAGS;
        popd
    done

    echo "*********************************"
    echo "* RTEMS tests compiled properly *"
    echo "********************************"
}

function create_zip {
    if [ ! -d ${RTEMS_INSTALL_DIR}/zip ]; then
        mkdir ${RTEMS_INSTALL_DIR}/zip;

        cp -R ${REPOSITORIES_DIR}/MovidiusRTEMS-tools \
        ${REPOSITORIES_DIR}/MovidiusRTEMS \
        ${REPOSITORIES_DIR}/MovidiusRTEMS-libbsd \
        ${RTEMS_INSTALL_DIR}/zip

        pushd ${RTEMS_INSTALL_DIR}/zip/MovidiusRTEMS-tools
        ./waf distclean
        for i in `find ./| grep "\.pyc$"`; do
            rm -rf $i;
        done
        rm -rf .git
        popd

        pushd ${RTEMS_INSTALL_DIR}/zip/MovidiusRTEMS
        ./bootstrap -c
        for i in `find ./| grep "\.h\.in$"`; do
            rm -rf $i;
        done
        #restore two files that were not meant to be removed
        git checkout cpukit/zlib/zconf.h.in tools/build/config.h.in
        rm -rf .git
        popd

        pushd ${RTEMS_INSTALL_DIR}/zip/MovidiusRTEMS-libbsd
        tools_waf=${REPOSITORIES_DIR}/MovidiusRTEMS-tools/waf
        $tools_waf distclean
        for i in `find ./| grep "\.pyc$"`; do
            rm -rf $i;
        done
        rm -rf .git
        popd

        pushd ${RTEMS_INSTALL_DIR}/zip
        for i in `find ./| grep "\.git"`; do
            rm -rf $i;
        done
        #create the actual zips
        zip -rq9 RTEMS.zip                  MovidiusRTEMS
        zip -rq9 RTEMS-tools.zip            MovidiusRTEMS MovidiusRTEMS-tools
        zip -rmq9 RTEMS-tools-libbsd.zip    MovidiusRTEMS MovidiusRTEMS-tools MovidiusRTEMS-libbsd
        echo "Zip files located in `pwd` :"
        echo "`ls`"

        popd
    fi
}

mkdir -p "${RTEMS_INSTALL_DIR}"
mkdir -p "${TOOLS_INSTALL_DIR}/bin"
mkdir -p "${REPOSITORIES_DIR}"
mkdir -p "${LOGGING_DIR}"

find_tools;
create_links;

export PATH=${TOOLS_INSTALL_DIR}/bin:${PATH}

if [ "$1" == "auto-tools" ] || [ "$1" == "all" ]; then
	if [ -f ${LOGGING_DIR}/autotools_complete ]; then
	    echo "Found Autotools; skipping..."
	else
	    build_autotools;
	    touch ${LOGGING_DIR}/autotools_complete
	fi
fi

if [ "$1" == "rtems-tools" ] || [ "$1" == "all" ]; then
	if [ -f ${LOGGING_DIR}/rtemstools_complete ]; then
	    echo "Found RTEMS tools; skipping..."
	else
	    build_rtemstools;
	    touch ${LOGGING_DIR}/rtemstools_complete
	fi
fi

if [ "$1" == "rtems" ] || [ "$1" == "all" ]; then
	if [ -f ${LOGGING_DIR}/rtems_complete ]; then
	    echo "Found RTEMS; skipping..."
	else
	    build_rtems;
	    touch ${LOGGING_DIR}/rtems_complete
	fi
fi
if [ "$1" == "rtems-debug" ]; then
	if [ -f ${LOGGING_DIR}/rtems_complete ]; then
	    echo "Found RTEMS; skipping..."
	else
	    EXTRA_OPT="--enable-rtems-debug"
	    build_rtems;
	    touch ${LOGGING_DIR}/rtems_complete
	fi
fi

if [ "$1" == "libbsd" ] || [ "$1" == "all" ]; then
	if [ -f ${LOGGING_DIR}/rtems_libbsd_complete ]; then
	    echo "Found RTEMS-libbsd; skipping..."
	else
	    build_rtems_libbsd;
	    touch ${LOGGING_DIR}/rtems_libbsd_complete
	fi
fi

if [ "$1" == "tests" ]; then
	if [ -f ${LOGGING_DIR}/rtems_tests_complete ]; then
	    echo "Found RTEMS TESTS; skipping..."
	else
	    build_rtems_tests;
	    touch ${LOGGING_DIR}/rtems_tests_complete
	fi
fi

if [ "$1" == "zip" ]; then
	if [ -f ${LOGGING_DIR}/zip_complete ]; then
	    echo "Found ZIP; skipping..."
	else
	if [ ! -f ${LOGGING_DIR}/rtemstools_complete ] ||
	   [ ! -f ${LOGGING_DIR}/rtems_complete ]      ||
	   [ ! -f ${LOGGING_DIR}/rtems_libbsd_complete ]; then
        echo "RTEMS is not properly configured yet";
        exit 1;
    fi

	    create_zip;
	    touch ${LOGGING_DIR}/zip_complete
	fi
fi
