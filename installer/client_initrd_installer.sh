#!/bin/bash

TFTPBOOT=/var/lib/tftpboot
OGROOT=/opt/opengnsys
INITRD=0
UPDATE=0

function arguments_parser
{
    while [ $# -gt 0 ];do
        case $1 in
            ("-t")
            shift
            if [ $# -eq 0 ];then
                echo "Error parseando argumentos"
                exit -1
            else
                OGROOT=$1
                shift
            fi
            ;;

            ("-s")
            shift
            if [ $# -eq 0 ]; then
                echo "Error parseando argumentos"
			exit -1
            else
                SVNROOT=$1
                shift
            fi
            ;;

            ("-u")
            shift
            UPDATE=1
            ;;

            ("-i")
            shift
            INITRD=1
            ;;
        esac
    done
}

function checking
{
    if [ $UID != 0 ]; then
        echo "No tiene permisos suficientes para ejecutar este script"
        exit -1
    fi
    if [ -z $SVNROOT ]; then
           echo "Necesito saber la ruta de las fuentes del proyecto."
           echo "$0 -s /ruta/hacia/las/fuentes"
           echo "Tambien puedes editar el script y anyadirlo manualmente."
           exit -1
    else
       if [ ! -d $SVNROOT/admin ] ||
          [ ! -d $SVNROOT/client ] ||
          [ ! -d $SVNROOT/doc ] ||
          [ ! -d $SVNROOT/repoman ] ||
          [ ! -d $SVNROOT/installer ] ||
          [ ! -d $SVNROOT/server ] ; then
           echo "La ruta dada para las fuentes del proyecto son incorrectas"
           exit -1;
       fi
    fi
}

function install_necesary_packages
{
    apt-get install pxe dhcp3-server tftpd-hpa nfs-kernel-server
}

function create_file_system
{
    mkdir -p $TFTPBOOT

    mkdir -p $OGROOT

    mkdir -p $OGROOT/bin
    mkdir -p $OGROOT/lib
    mkdir -p $OGROOT/images
    mkdir -p $OGROOT/client
    mkdir -p $OGROOT/client/lib/engine/bin

    mkdir -p /etc/opengnsys
    mkdir -p /var/log/opengnsys/clients

    ln -fs $TFTPBOOT $OGROOT/tftpboot
    ln -fs /etc/opengnsys $OGROOT/etc
    ln -fs /var/log/opengnsys $OGROOT/log

    cp -ar $SVNROOT/client/nfsexport/* $OGROOT/client
    cp -ar $SVNROOT/client/engine/*.lib $OGROOT/client/lib/engine/bin
    cp -ar $SVNROOT/client/engine/*.sh $OGROOT/client/lib/engine/bin
}

function install_dhcpd
{
    cat $SVNROOT/server/DHCP/dhcpd.conf >> /etc/dhcp3/dhcpd.conf
    /etc/init.d/dhcp3-server restart
    echo "Revise el archivo /etc/dhcp3/dhcpd.conf para configurarlo para su red"
}

function install_tftpboot
{
    mkdir -p $OGROOT/tftpboot/pxelinux.cfg/
    cat $SVNROOT/server/PXE/pxelinux.cfg/default >> $OGROOT/tftpboot/pxelinux.cfg/default
}

function install_initrd
{
    $SVNROOT/client/boot/initrd-generator -t $OGROOT/tftpboot/
}

function install_nfsexport
{
    cat $SVNROOT/server/NFS/exports >> /etc/exports
    /etc/init.d/nfs-kernel-server restart

    echo "Revise el archivo /etc/exports para configurarlo para su red"
}

arguments_parser $@
checking

if [ $INITRD -eq 1 ]; then
    install_initrd
fi

if [ $UPDATE -eq 1 ]; then
    create_file_system
else
    install_necesary_packages
    create_file_system
    install_dhcpd
    install_tftpboot
    install_initrd
    install_nfsexport
fi