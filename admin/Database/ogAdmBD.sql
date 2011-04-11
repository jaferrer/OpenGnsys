-- phpMyAdmin SQL Dump
-- version 3.2.2.1deb1
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 20-09-2010 a las 22:43:50
-- Versión del servidor: 5.1.37
-- Versión de PHP: 5.2.10-2ubuntu6.4

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Base de datos: `ogAdmBD`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `acciones`
--

CREATE TABLE IF NOT EXISTS `acciones` (
  `idaccion` int(11) NOT NULL AUTO_INCREMENT,
  `tipoaccion` smallint(6) NOT NULL,
  `idtipoaccion` int(11) NOT NULL,
  `descriaccion` varchar(250) NOT NULL,
  `idordenador` int(11) NOT NULL,
  `ip` varchar(50) NOT NULL,
  `sesion` int(11) NOT NULL,
  `idcomando` int(11) NOT NULL,
  `parametros` text,
  `fechahorareg` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `fechahorafin` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `estado` tinyint(1) NOT NULL DEFAULT '0',
  `resultado` tinyint(1) NOT NULL DEFAULT '0',
  `descrinotificacion` varchar(256) DEFAULT NULL,
  `ambito` smallint(6) NOT NULL DEFAULT '0',
  `idambito` int(11) NOT NULL DEFAULT '0',
  `restrambito` text NOT NULL,
  `idprocedimiento` int(11) NOT NULL DEFAULT '0',
  `idtarea` int(11) NOT NULL DEFAULT '0',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `idprogramacion` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idaccion`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `acciones_menus`
--

CREATE TABLE IF NOT EXISTS `acciones_menus` (
  `idaccionmenu` int(11) NOT NULL AUTO_INCREMENT,
  `tipoaccion` tinyint(4) NOT NULL DEFAULT '0',
  `idtipoaccion` int(11) NOT NULL DEFAULT '0',
  `idmenu` int(11) NOT NULL DEFAULT '0',
  `tipoitem` tinyint(4) DEFAULT NULL,
  `idurlimg` int(11) DEFAULT NULL,
  `descripitem` varchar(250) DEFAULT NULL,
  `orden` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`idaccionmenu`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `administradores_centros`
--

CREATE TABLE IF NOT EXISTS `administradores_centros` (
  `idadministradorcentro` int(11) NOT NULL AUTO_INCREMENT,
  `idusuario` int(11) NOT NULL DEFAULT '0',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idadministradorcentro`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `aulas`
--

CREATE TABLE IF NOT EXISTS `aulas` (
  `idaula` int(11) NOT NULL AUTO_INCREMENT,
  `nombreaula` varchar(100) NOT NULL DEFAULT '',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `urlfoto` varchar(250) DEFAULT NULL,
  `cuadro_y` char(3) DEFAULT NULL,
  `cuadro_x` char(3) DEFAULT NULL,
  `cagnon` tinyint(1) DEFAULT NULL,
  `pizarra` tinyint(1) DEFAULT NULL,
  `grupoid` int(11) DEFAULT NULL,
  `ubicacion` varchar(255) DEFAULT NULL,
  `comentarios` text,
  `puestos` tinyint(4) DEFAULT NULL,
  `horaresevini` tinyint(4) DEFAULT NULL,
  `horaresevfin` tinyint(4) DEFAULT NULL,
  `modomul` tinyint(4) NOT NULL,
  `ipmul` varchar(16) NOT NULL,
  `pormul` int(11) NOT NULL,
  `velmul` smallint(6) NOT NULL DEFAULT '70',
  `router` VARCHAR( 30 ),
  `netmask` VARCHAR( 30 ),
  `dns` VARCHAR (30),
  `modp2p` enum('seeder','peer','leecher') DEFAULT 'peer',
  `timep2p` INT(11) NOT NULL DEFAULT '60',
  PRIMARY KEY (`idaula`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;



--
-- Estructura de tabla para la tabla `asistentes`
--

CREATE TABLE IF NOT EXISTS `asistentes` (
  `idcomando` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `pagina` varchar(256) NOT NULL,
  `gestor` varchar(256) NOT NULL,
  `funcion` varchar(64) NOT NULL,
  `urlimg` varchar(250) DEFAULT NULL,
  `aplicambito` tinyint(4) DEFAULT NULL,
  `visuparametros` varchar(250) DEFAULT NULL,
  `parametros` varchar(250) DEFAULT NULL,
  `comentarios` text,
  `activo` tinyint(1) NOT NULL,
  PRIMARY KEY ( `idcomando` , `descripcion` ) 
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=11 ;


INSERT INTO `asistentes` (`idcomando`, `descripcion`, `pagina`, `gestor`, `funcion`, `urlimg`, `aplicambito`, `visuparametros`, `parametros`, `comentarios`, `activo`) VALUES
('8', 'Asistente Clonacion Particiones Remotas', '../asistentes/AsistenteCloneRemotePartition.php', '../asistentes/gestores/gestor_Comandos.php', 'EjecutarScript', ' ', '31', 'iph;tis;dcr;dsp', 'nfn;iph;tis;dcr;scp', ' ', '1'),
('8', 'Asistente "Deploy" de Imagenes', '../asistentes/AsistenteDeployImage.php', '../asistentes/gestores/gestor_Comandos.php', 'EjecutarScript', ' ', '31', 'iph;tis;dcr;dsp', 'nfn;iph;tis;dcr;scp', ' ', '1'),
('8', 'Asistente "UpdateCache" con Imagenes', '../asistentes/AsistenteUpdateCache.php', '../asistentes/gestores/gestor_Comandos.php', 'EjecutarScript', ' ', '31', 'iph;tis;dcr;dsp', 'nfn;iph;tis;dcr;scp', ' ', '0'),
('8', 'Asistente Restauracion de Imagenes', '../asistentes/AsistenteRestoreImage.php', '../asistentes/gestores/gestor_Comandos.php', 'EjecutarScript', ' ', '31', 'iph;tis;dcr;dsp', 'nfn;iph;tis;dcr;scp', ' ', '0'),
('8', 'Asistente Particionado', '../asistentes/AsistenteParticionado.php', '../asistentes/gestores/gestor_Comandos.php', 'EjecutarScript', ' ', '31', 'iph;tis;dcr;dsp', 'nfn;iph;tis;dcr;scp', ' ', '1');



-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `campus`
--

CREATE TABLE IF NOT EXISTS `campus` (
  `idcampus` int(11) NOT NULL AUTO_INCREMENT,
  `nombrecampus` varchar(100) NOT NULL DEFAULT '',
  `iduniversidad` int(11) DEFAULT NULL,
  `urlmapa` varchar(255) DEFAULT NULL,
  `cuadro_y` tinyint(3) DEFAULT NULL,
  `cuadro_x` tinyint(3) DEFAULT NULL,
  PRIMARY KEY (`idcampus`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

--
-- Volcar la base de datos para la tabla `campus`
--


-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `centros`
--

CREATE TABLE IF NOT EXISTS `centros` (
  `idcentro` int(11) NOT NULL AUTO_INCREMENT,
  `nombrecentro` varchar(100) NOT NULL DEFAULT '',
  `identidad` int(11) DEFAULT NULL,
  `comentarios` text,
  PRIMARY KEY (`idcentro`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;


	--
	-- Volcar la base de datos para la tabla `centros`
	--
	INSERT INTO `centros` (`idcentro`,`nombrecentro`,`identidad`,`comentarios`) VALUES 
	 (1,'Unidad organizativa (Default)',1,'Esta Unidad Organizativa se crea automáticamente en el proceso de instalación de OpenGNSys');
	 
	
-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `comandos`
--

CREATE TABLE IF NOT EXISTS `comandos` (
  `idcomando` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `pagina` varchar(256) NOT NULL,
  `gestor` varchar(256) NOT NULL,
  `funcion` varchar(64) NOT NULL,
  `urlimg` varchar(250) DEFAULT NULL,
  `aplicambito` tinyint(4) DEFAULT NULL,
  `visuparametros` varchar(250) DEFAULT NULL,
  `parametros` varchar(250) DEFAULT NULL,
  `comentarios` text,
  `activo` tinyint(1) NOT NULL,
  PRIMARY KEY (`idcomando`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=11 ;

--
-- Volcar la base de datos para la tabla `comandos`
--

INSERT INTO `comandos` (`idcomando`, `descripcion`, `pagina`, `gestor`, `funcion`, `urlimg`, `aplicambito`, `visuparametros`, `parametros`, `comentarios`, `activo`) VALUES
(1, 'Arrancar', '../comandos/Arrancar.php', '../comandos/gestores/gestor_Comandos.php', 'Arrancar', '', 31, '', 'nfn;iph;mac', '', 1),
(2, 'Apagar', '../comandos/Apagar.php', '../comandos/gestores/gestor_Comandos.php', 'Apagar', '', 31, '', 'nfn;iph;mac', '', 1),
(3, 'Restaurar Imagen', '../comandos/RestaurarImagen.php', '../comandos/gestores/gestor_Comandos.php', 'RestaurarImagen', '', 28, 'dsk;par;idi;nci;ipr;ptc', 'nfn;iph;mac;dsk;par;idi;nci;ipr;ifs;ptc', '', 1),
(4, 'Crear Imagen', '../comandos/CrearImagen.php', '../comandos/gestores/gestor_Comandos.php', 'CrearImagen', '', 16, 'dsk;par;idi;nci;ipr;cpt', 'nfn;iph;mac;dsk;par;idi;nci;ipr;cpt;', '', 1),
(5, 'Reiniciar', '../comandos/Reiniciar.php', '../comandos/gestores/gestor_Comandos.php', 'Reiniciar', '', 31, '', 'nfn;iph;mac;', '', 1),
(6, 'Inventario Hardware', '../comandos/InventarioHardware.php', '../comandos/gestores/gestor_Comandos.php', 'InventarioHardware', '', 16, '', 'nfn;iph;mac;', '', 1),
(7, 'Inventario Software', '../comandos/InventarioSoftware.php', '../comandos/gestores/gestor_Comandos.php', 'InventarioSoftware', '', 16, 'par', 'nfn;iph;mac;par', '', 1),
(8, 'Ejecutar Script', '../comandos/EjecutarScripts.php', '../comandos/gestores/gestor_Comandos.php', 'EjecutarScript', '', 31, 'iph;tis;dcr;scp', 'nfn;iph;tis;dcr;scp', '', 1),
(9, 'Iniciar Sesion', '../comandos/IniciarSesion.php', '../comandos/gestores/gestor_Comandos.php', 'IniciarSesion', '', 31, 'par', 'nfn;iph;par', '', 1),
(10, 'Particionar y Formatear', '../comandos/Configurar.php', '../comandos/gestores/gestor_Comandos.php', 'Configurar', '', 28, 'dsk;cfg;', 'nfn;iph;mac;dsk;cfg;par;cpt;sfi;tam;ope', '', 0);



-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `entidades`
--

CREATE TABLE IF NOT EXISTS `entidades` (
  `identidad` int(11) NOT NULL AUTO_INCREMENT,
  `nombreentidad` varchar(200) NOT NULL DEFAULT '',
  `comentarios` text,
  `iduniversidad` int(11) DEFAULT NULL,
  `grupoid` int(11) DEFAULT NULL,
  PRIMARY KEY (`identidad`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Volcar la base de datos para la tabla `entidades`
--

INSERT INTO `entidades` (`identidad`, `nombreentidad`, `comentarios`, `iduniversidad`, `grupoid`) VALUES
(1, 'Entidad (Default)', 'Esta Entidad se crea automáticamente en el proceso de instalación de OpenGNSys', 1, 0);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `entornos`
--

CREATE TABLE IF NOT EXISTS `entornos` (
  `identorno` int(11) NOT NULL AUTO_INCREMENT,
  `ipserveradm` varchar(50) NOT NULL,
  `portserveradm` int(20) NOT NULL,
  `protoclonacion` varchar(50) NOT NULL,
  PRIMARY KEY (`identorno`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Volcar la base de datos para la tabla `entornos`
--

INSERT INTO `entornos` (`identorno`, `ipserveradm`, `portserveradm`, `protoclonacion`) VALUES
(1, 'SERVERIP', 2008, 'UNICAST');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `estatus`
--

CREATE TABLE IF NOT EXISTS `estatus` (
  `idestatus` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  PRIMARY KEY (`idestatus`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

--
-- Volcar la base de datos para la tabla `estatus`
--

INSERT INTO `estatus` (`idestatus`, `descripcion`) VALUES
(1, 'P.D.I. ( Profesor)'),
(2, 'P.A.S.'),
(3, 'Doctor'),
(4, 'Alumno'),
(5, 'Otros');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `grupos`
--

CREATE TABLE IF NOT EXISTS `grupos` (
  `idgrupo` int(11) NOT NULL AUTO_INCREMENT,
  `nombregrupo` varchar(250) NOT NULL DEFAULT '',
  `grupoid` int(11) NOT NULL DEFAULT '0',
  `tipo` tinyint(4) NOT NULL DEFAULT '0',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `iduniversidad` int(11) DEFAULT NULL,
  `comentarios` text,
  PRIMARY KEY (`idgrupo`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `gruposordenadores`
--

CREATE TABLE IF NOT EXISTS `gruposordenadores` (
  `idgrupo` int(11) NOT NULL AUTO_INCREMENT,
  `nombregrupoordenador` varchar(250) NOT NULL DEFAULT '',
  `idaula` int(11) NOT NULL DEFAULT '0',
  `grupoid` int(11) DEFAULT NULL,
  `comentarios` text,
  PRIMARY KEY (`idgrupo`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `hardwares`
--

CREATE TABLE IF NOT EXISTS `hardwares` (
  `idhardware` int(11) NOT NULL AUTO_INCREMENT,
  `idtipohardware` int(11) NOT NULL DEFAULT '0',
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `grupoid` int(11) DEFAULT NULL,
  PRIMARY KEY (`idhardware`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;


-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `iconos`
--

CREATE TABLE IF NOT EXISTS `iconos` (
  `idicono` int(11) NOT NULL AUTO_INCREMENT,
  `urlicono` varchar(200) DEFAULT NULL,
  `idtipoicono` int(11) DEFAULT NULL,
  `descripcion` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`idicono`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=15 ;

--
-- Volcar la base de datos para la tabla `iconos`
--

INSERT INTO `iconos` (`idicono`, `urlicono`, `idtipoicono`, `descripcion`) VALUES
(1, 'vga.gif', 1, 'Tarjeta gráfica'),
(2, 'nic.gif', 1, 'Tarjeta de Red'),
(3, 'placabase.gif', 1, 'Placas base'),
(4, 'tsonido.gif', 1, 'Tarjeta de sonido'),
(5, 'camweb.gif', 1, 'Cámara web'),
(6, 'logoXP.png', 2, 'Logo Windows XP'),
(7, 'logolinux.png', 2, 'Logo General de Linux'),
(8, 'particionar.png', 2, 'Particionar'),
(9, 'ordenadoroff.png', 2, 'Ordenador apagado'),
(10, 'ordenadoron.png', 2, 'Ordenador encendido'),
(11, 'usb.gif', 1, 'Mi icono usb'),
(12, 'ide.gif', 1, 'Controladores IDE'),
(13, 'dvdcd.gif', 1, 'Lectoras y grabadoras de DVD'),
(14, 'audio.gif', 1, 'Dispositivos de audio');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `idiomas`
--

CREATE TABLE IF NOT EXISTS `idiomas` (
  `ididioma` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(100) DEFAULT NULL,
  `nemonico` char(3) DEFAULT NULL,
  PRIMARY KEY (`ididioma`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Volcar la base de datos para la tabla `idiomas`
--

INSERT INTO `idiomas` (`ididioma`, `descripcion`, `nemonico`) VALUES
(1, 'Español', 'esp'),
(2, 'Ingles', 'eng'),
(3, 'Catalan', 'cat');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `imagenes`
--

CREATE TABLE IF NOT EXISTS `imagenes` (
  `idimagen` int(11) NOT NULL AUTO_INCREMENT,
  `nombreca` varchar(50) NOT NULL,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `idperfilsoft` int(11) DEFAULT NULL,
  `idcentro` int(11) DEFAULT NULL,
  `comentarios` text,
  `grupoid` int(11) DEFAULT NULL,
  `idrepositorio` int(11) NOT NULL,
  `numpar` smallint(6) NOT NULL,
  `codpar` smallint(6) NOT NULL,
  PRIMARY KEY (`idimagen`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `menus`
--

CREATE TABLE IF NOT EXISTS `menus` (
  `idmenu` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `idurlimg` int(11) NOT NULL DEFAULT '0',
  `titulo` varchar(250) DEFAULT NULL,
  `coorx` int(11) DEFAULT NULL,
  `coory` int(11) DEFAULT NULL,
  `modalidad` tinyint(4) DEFAULT NULL,
  `scoorx` int(11) DEFAULT NULL,
  `scoory` int(11) DEFAULT NULL,
  `smodalidad` tinyint(4) DEFAULT NULL,
  `comentarios` text,
  `grupoid` int(11) NOT NULL DEFAULT '0',
  `htmlmenupub` varchar(250) DEFAULT NULL,
  `htmlmenupri` varchar(250) DEFAULT NULL,
  `resolucion` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`idmenu`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `nombresos`
--

CREATE TABLE IF NOT EXISTS `nombresos` (
  `idnombreso` smallint(11) NOT NULL AUTO_INCREMENT,
  `nombreso` varchar(250) NOT NULL,
  `idtiposo` int(11) DEFAULT '0',
  PRIMARY KEY (`idnombreso`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ordenadores`
--

CREATE TABLE IF NOT EXISTS `ordenadores` (
  `idordenador` int(11) NOT NULL AUTO_INCREMENT,
  `nombreordenador` varchar(100) DEFAULT NULL,
  `ip` varchar(16) NOT NULL,
  `mac` varchar(12) DEFAULT NULL,
  `idaula` int(11) DEFAULT NULL,
  `idperfilhard` int(11) DEFAULT NULL,
  `idrepositorio` int(11) DEFAULT NULL,
  `grupoid` int(11) DEFAULT NULL,
  `idmenu` int(11) DEFAULT NULL,
  `cache` int(11) DEFAULT NULL,
  `router` varchar(16) NOT NULL,
  `mascara` varchar(16) NOT NULL,
  `idproautoexec` int(11) NOT NULL,
  `arranque` VARCHAR( 30 ) NOT NULL DEFAULT '1',
  `netiface` enum('eth0','eth1','eth2') DEFAULT 'eth0',
  `netdriver` VARCHAR( 30 ) NOT NULL DEFAULT 'generic',
  PRIMARY KEY (`idordenador`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;



-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ordenadores_particiones`
--

CREATE TABLE IF NOT EXISTS `ordenadores_particiones` (
  `idordenador` int(11) NOT NULL,
  `numpar` tinyint(4) NOT NULL,
  `codpar` smallint(11) NOT NULL,
  `tamano` int(11) NOT NULL,
  `idsistemafichero` smallint(11) NOT NULL,
  `idnombreso` smallint(11) NOT NULL,
  `idimagen` int(11) NOT NULL,
  `idperfilsoft` int(11) NOT NULL,
  UNIQUE KEY `idordenadornumpar` (`idordenador`,`numpar`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `parametros`
--

CREATE TABLE IF NOT EXISTS `parametros` (
  `idparametro` int(11) NOT NULL AUTO_INCREMENT,
  `nemonico` char(3) NOT NULL,
  `descripcion` text NOT NULL,
  `nomidentificador` varchar(64) NOT NULL,
  `nomtabla` varchar(64) NOT NULL,
  `nomliteral` varchar(64) NOT NULL,
  `tipopa` tinyint(1) DEFAULT '0',
  `visual` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idparametro`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=31 ;

--
-- Volcar la base de datos para la tabla `parametros`
--

INSERT INTO `parametros` (`idparametro`, `nemonico`, `descripcion`, `nomidentificador`, `nomtabla`, `nomliteral`, `tipopa`, `visual`) VALUES
(1, 'nfn', 'Nombre de la función o script a ejecutar en el cliente y que implementa el comando. Es posible que también els ervidor debo ejecutar la misma función como ocurre en el comando "Arrancar" y que implementa el comportamiento del comando en el servidor', '', '', '', 0, 0),
(2, 'iph', 'Dirección ip de los ordenadores a los que se envía el comando', '', '', '', 0, 0),
(3, 'ido', 'Identificadores de los ordenadores a los que se envía el comando', '', '', '', 0, 0),
(4, 'mac', 'Direcciones macs de los clientes a los que se le envía el comando', '', '', '', 0, 0),
(5, 'idc', 'Unidad organizativa', 'idcentro', 'centros', '', 1, 0),
(6, 'ida', 'Aula', 'idaula', 'aulas', 'nombreaula', 1, 0),
(18, 'cfg', 'Configuración', '', '', '', 2, 1),
(7, 'dsk', 'Disco', '', '', '', 0, 1),
(8, 'par', 'Partición', '', '', '', 0, 1),
(9, 'ifh', 'Perfil Hardware', 'idperfilhard', 'perfileshard', 'descripcion', 1, 1),
(10, 'ifs', 'Perfil Software', 'idperfilsoft', 'perfilessoft', 'descripcion', 1, 1),
(11, 'idi', 'Imagen', 'idimagen', 'imagenes', 'descripcion', 1, 1),
(12, 'nci', 'Nombre canonico', '', '', '', 0, 1),
(13, 'scp', 'Código a ejecutar en formato script', '', '', '', 0, 0),
(14, 'npc', 'Nombre del cliente', '', '', '', NULL, 0),
(15, 'che', 'Tamaño de la cache del cliente', '', '', '', NULL, 0),
(16, 'exe', 'Identificador del procedimiento que será el que ejecute el cliente al arrancar (Autoexec)', '', '', '', 0, 0),
(17, 'res', 'Respuesta del comando: Puede tomar los valores 1 o 2 en el caso de que la respuesta sea correcta o que haya un error al ejecutarse.', '', '', '', 0, 0),
(19, 'ipr', 'Repositorio', 'ip', 'repositorios', 'nombrerepositorio', 1, 1),
(20, 'cpt', 'Tipo partición', 'codpar', 'tipospar', 'tipopar', 1, 1),
(21, 'sfi', 'Sistema de fichero', 'idsistemafichero', 'sistemasficheros', 'nemonico', 1, 0),
(22, 'tam', 'Tamaño', '', '', '', 0, 0),
(23, 'ope', 'Operación', ';', '', 'Sin operación;Formatear;Ocultar;Mostrar', 3, 1),
(24, 'nfl', 'Nombre del fichero que se envía o se recibe', '', '', '', 0, 0),
(25, 'hrd', 'Nombre del archivo de inventario hardware enviado por la red', '', '', '', 0, 0),
(26, 'sft', 'Nombre del archivo de inventario software enviado por la red', '', '', '', 0, 0),
(27, 'tpc', 'Tipo de cliente', '', '', '', 0, 0),
(28, 'scp', 'Código script', '', '', '', 4, 1),
(30, 'ptc', 'Protocolo de clonación', ';', '', ';Unicast;Multicast;Torrent', 3, 1);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `perfileshard`
--

CREATE TABLE IF NOT EXISTS `perfileshard` (
  `idperfilhard` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `comentarios` text,
  `grupoid` int(11) DEFAULT NULL,
  `idcentro` int(11) NOT NULL,
  PRIMARY KEY (`idperfilhard`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;


-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `perfileshard_hardwares`
--

CREATE TABLE IF NOT EXISTS `perfileshard_hardwares` (
  `idperfilhard` int(11) NOT NULL DEFAULT '0',
  `idhardware` int(11) NOT NULL DEFAULT '0',
  KEY `idperfilhard` (`idperfilhard`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `perfilessoft`
--

CREATE TABLE IF NOT EXISTS `perfilessoft` (
  `idperfilsoft` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `comentarios` text,
  `grupoid` int(11) DEFAULT NULL,
  `idcentro` int(11) NOT NULL,
  PRIMARY KEY (`idperfilsoft`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `perfilessoft_softwares`
--

CREATE TABLE IF NOT EXISTS `perfilessoft_softwares` (
  `idperfilsoft` int(11) NOT NULL DEFAULT '0',
  `idsoftware` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `plataformas`
--

CREATE TABLE IF NOT EXISTS `plataformas` (
  `idplataforma` int(11) NOT NULL AUTO_INCREMENT,
  `plataforma` varchar(250) NOT NULL,
  PRIMARY KEY (`idplataforma`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=18 ;

--
-- Volcar la base de datos para la tabla `plataformas`
--

INSERT INTO `plataformas` (`idplataforma`, `plataforma`) VALUES
(1, 'MsDos'),
(2, 'Windows'),
(3, 'Linux'),
(4, 'Mac'),
(5, 'OS');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `procedimientos`
--

CREATE TABLE IF NOT EXISTS `procedimientos` (
  `idprocedimiento` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `urlimg` varchar(250) DEFAULT NULL,
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `comentarios` text,
  `grupoid` int(11) DEFAULT '0',
  PRIMARY KEY (`idprocedimiento`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `procedimientos_acciones`
--

CREATE TABLE IF NOT EXISTS `procedimientos_acciones` (
  `idprocedimientoaccion` int(11) NOT NULL AUTO_INCREMENT,
  `idprocedimiento` int(11) NOT NULL DEFAULT '0',
  `orden` smallint(4) DEFAULT NULL,
  `idcomando` int(11) NOT NULL DEFAULT '0',
  `parametros` text,
  `procedimientoid` int(11) NOT NULL,
  PRIMARY KEY (`idprocedimientoaccion`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `programaciones`
--

CREATE TABLE IF NOT EXISTS `programaciones` (
  `idprogramacion` int(11) NOT NULL AUTO_INCREMENT,
  `tipoaccion` int(11) DEFAULT NULL,
  `identificador` int(11) DEFAULT NULL,
  `nombrebloque` varchar(255) DEFAULT NULL,
  `annos` tinyint(4) DEFAULT NULL,
  `meses` smallint(4) DEFAULT NULL,
  `diario` int(11) DEFAULT NULL,
  `dias` tinyint(4) DEFAULT NULL,
  `semanas` tinyint(4) DEFAULT NULL,
  `horas` smallint(4) DEFAULT NULL,
  `ampm` tinyint(1) DEFAULT NULL,
  `minutos` tinyint(4) DEFAULT NULL,
  `segundos` tinyint(4) DEFAULT NULL,
  `horasini` smallint(4) DEFAULT NULL,
  `ampmini` tinyint(1) DEFAULT NULL,
  `minutosini` tinyint(4) DEFAULT NULL,
  `horasfin` smallint(4) DEFAULT NULL,
  `ampmfin` tinyint(1) DEFAULT NULL,
  `minutosfin` tinyint(4) DEFAULT NULL,
  `suspendida` tinyint(1) DEFAULT NULL,
  `sesion` int(11) NOT NULL,
  PRIMARY KEY (`idprogramacion`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `repositorios`
--

CREATE TABLE IF NOT EXISTS `repositorios` (
  `idrepositorio` int(11) NOT NULL AUTO_INCREMENT,
  `nombrerepositorio` varchar(250) NOT NULL,
  `ip` varchar(15) NOT NULL DEFAULT '',
  `passguor` varchar(50) NOT NULL DEFAULT '',
  `pathrepoconf` varchar(250) NOT NULL,
  `pathrepod` varchar(250) NOT NULL,
  `pathpxe` varchar(250) NOT NULL,
  `idcentro` int(11) DEFAULT NULL,
  `grupoid` int(11) DEFAULT NULL,
  `comentarios` text,
  `puertorepo` int(11) NOT NULL,
  PRIMARY KEY (`idrepositorio`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

INSERT INTO `repositorios` (`idrepositorio`,`nombrerepositorio`,`ip`,`passguor`,`pathrepoconf`,`pathrepod`,`pathpxe`,`idcentro`,`grupoid`,`comentarios`,`puertorepo`) VALUES 
 (1,'Repositorio (Default)','SERVERIP','','','/opt/opengnsys/admin','/opt/opengnsys/tftpboot/pxelinux.cfg',1,0,'',2002);


-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `sistemasficheros`
--

CREATE TABLE IF NOT EXISTS `sistemasficheros` (
  `idsistemafichero` smallint(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(50) NOT NULL DEFAULT '',
  `nemonico` varchar(16) DEFAULT NULL,
  `codpar` smallint(6) NOT NULL,
  PRIMARY KEY (`idsistemafichero`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `softwares`
--

CREATE TABLE IF NOT EXISTS `softwares` (
  `idsoftware` int(11) NOT NULL AUTO_INCREMENT,
  `idtiposoftware` int(11) NOT NULL DEFAULT '0',
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `urlimg` varchar(250) DEFAULT NULL,
  `idtiposo` int(11) DEFAULT NULL,
  `grupoid` int(11) DEFAULT NULL,
  PRIMARY KEY (`idsoftware`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tareas`
--

CREATE TABLE IF NOT EXISTS `tareas` (
  `idtarea` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `urlimg` varchar(250) DEFAULT NULL,
  `idcentro` int(11) NOT NULL DEFAULT '0',
  `ambito` smallint(6) NOT NULL DEFAULT '0',
  `idambito` int(11) NOT NULL DEFAULT '0',
  `restrambito` text NOT NULL,
  `comentarios` text,
  `grupoid` int(11) DEFAULT '0',
  PRIMARY KEY (`idtarea`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tareas_acciones`
--

CREATE TABLE IF NOT EXISTS `tareas_acciones` (
  `idtareaaccion` int(11) NOT NULL AUTO_INCREMENT,
  `idtarea` int(11) NOT NULL DEFAULT '0',
  `orden` smallint(6) NOT NULL DEFAULT '0',
  `idprocedimiento` int(11) NOT NULL DEFAULT '0',
  `tareaid` int(11) DEFAULT '0',
  PRIMARY KEY (`idtareaaccion`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tipohardwares`
--

CREATE TABLE IF NOT EXISTS `tipohardwares` (
  `idtipohardware` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `urlimg` varchar(250) NOT NULL DEFAULT '',
  `nemonico` char(3) NOT NULL,
  `pci` tinyint(1) NOT NULL,
  PRIMARY KEY (`idtipohardware`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=17 ;

--
-- Volcar la base de datos para la tabla `tipohardwares`
--

INSERT INTO `tipohardwares` (`idtipohardware`, `descripcion`, `urlimg`, `nemonico`, `pci`) VALUES
(1, 'Placas', '../images/iconos/placabase.gif', 'boa', 0),
(2, 'Dispositivos Multimedia', '../images/iconos/tsonido.gif', 'mul', 0),
(3, 'Tarjetas de Red', '../images/iconos/nic.gif', 'net', 0),
(4, 'Microprocesadores', '../images/iconos/micro.gif', 'cpu', 0),
(5, 'Memorias', '../images/iconos/confihard.gif', 'mem', 0),
(7, 'Tarjetas gráficas', '../images/iconos/vga.gif', 'vga', 0),
(8, 'Discos', '../images/iconos/discoduro.gif', 'dis', 0),
(9, 'Dispositivos de sonido', '../images/iconos/audio.gif', 'aud', 0),
(10, 'Marca y modelo del equipo', '../images/iconos/confihard.gif', 'mod', 0),
(11, 'Modelo y version de la bios', '../images/iconos/confihard.gif', 'bio', 0),
(12, 'Modelo de grabadora o  grabadora de CD/DVD', '../images/iconos/dvdcd.gif', 'cdr', 0),
(13, 'Controladores IDE', '../images/iconos/ide.gif', 'ide', 0),
(14, 'Controladores FireWire', '../images/iconos/confihard.gif', 'fir', 0),
(15, 'Controladores USB', '../images/iconos/usb.gif', 'usb', 0),
(16, 'Bus del Sistema', '../images/iconos/confihard.gif', 'bus', 0);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tiposoftwares`
--

CREATE TABLE IF NOT EXISTS `tiposoftwares` (
  `idtiposoftware` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  `urlimg` varchar(250) NOT NULL DEFAULT '',
  PRIMARY KEY (`idtiposoftware`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Volcar la base de datos para la tabla `tiposoftwares`
--

INSERT INTO `tiposoftwares` (`idtiposoftware`, `descripcion`, `urlimg`) VALUES
(1, 'Sistemas Operativos', '../images/iconos/so.gif'),
(2, 'Aplicaciones', '../images/iconos/aplicaciones.gif'),
(3, 'Archivos', '../images/iconos/archivos.gif');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tiposos`
--

CREATE TABLE IF NOT EXISTS `tiposos` (
  `idtiposo` int(11) NOT NULL AUTO_INCREMENT,
  `tiposo` varchar(250) NOT NULL,
  `idplataforma` int(11) NOT NULL,
  PRIMARY KEY (`idtiposo`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=21 ;

--
-- Volcar la base de datos para la tabla `tiposos`
--

INSERT INTO `tiposos` (`idtiposo`, `tiposo`, `idplataforma`) VALUES
(1, 'MsDos 6.0', 1),
(2, 'Windows 98', 2),
(3, 'Linux Ubuntu', 3),
(4, 'Mac', 0),
(5, 'OS', 0),
(17, 'Windows XP', 2),
(18, 'Windows Vista', 2),
(19, 'Linux Red Hat', 3),
(20, 'Windows 7', 2);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tipospar`
--

CREATE TABLE IF NOT EXISTS `tipospar` (
  `codpar` smallint(4) NOT NULL,
  `tipopar` varchar(250) NOT NULL,
  `clonable` tinyint(4) NOT NULL,
  UNIQUE KEY `codpar` (`codpar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `tipospar`
--

INSERT INTO `tipospar` (`codpar`, `tipopar`, `clonable`) VALUES
(0, 'EMPTY', 0),
(1, 'FAT12', 1),
(5, 'EXTENDED', 0),
(7, 'NTFS', 1),
(11, 'FAT32', 1),
(17, 'HFAT12', 1),
(22, 'HFAT16', 1),
(23, 'HNTFS', 1),
(27, 'HFAT32', 1),
(130, 'LINUX-SWAP', 0),
(131, 'LINUX', 1),
(142, 'LINUX-LVM', 1),
(191, 'SOLARIS', 1),
(202, 'CACHE', 0),
(253, 'LINUX-RAID', 1);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `universidades`
--

CREATE TABLE IF NOT EXISTS `universidades` (
  `iduniversidad` int(11) NOT NULL AUTO_INCREMENT,
  `nombreuniversidad` varchar(200) NOT NULL DEFAULT '',
  `comentarios` text,
  PRIMARY KEY (`iduniversidad`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Volcar la base de datos para la tabla `universidades`
--

INSERT INTO `universidades` (`iduniversidad`, `nombreuniversidad`, `comentarios`) VALUES
(1, 'Universidad default...', 'Esta Universidad se crea automáticamentese en el proceso de instalación de OpenGNSys');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `urlimagesitems`
--

CREATE TABLE IF NOT EXISTS `urlimagesitems` (
  `idurlimagesitems` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(250) NOT NULL DEFAULT '',
  PRIMARY KEY (`idurlimagesitems`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

--
-- Volcar la base de datos para la tabla `urlimagesitems`
--


-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `usuarios`
--

CREATE TABLE IF NOT EXISTS `usuarios` (
  `idusuario` int(11) NOT NULL AUTO_INCREMENT,
  `usuario` varchar(50) NOT NULL DEFAULT '',
  `pasguor` varchar(50) NOT NULL DEFAULT '',
  `nombre` varchar(200) DEFAULT NULL,
  `email` varchar(200) DEFAULT NULL,
  `ididioma` int(11) DEFAULT NULL,
  `idtipousuario` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`idusuario`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Volcar la base de datos para la tabla `usuarios`
--

INSERT INTO `usuarios` (`idusuario`, `usuario`, `pasguor`, `nombre`, `email`, `ididioma`, `idtipousuario`) VALUES
(1, 'DBUSER', 'DBPASSWORD', 'Usuario de la base de datos MySql', '', 1, 1);


CREATE TABLE IF NOT EXISTS `itemboot` (
  `label` varchar(50) collate utf8_spanish_ci NOT NULL,
  `kernel` varchar(100) collate utf8_spanish_ci NOT NULL,
  `append` varchar(500) collate utf8_spanish_ci NOT NULL,
  PRIMARY KEY  (`label`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;



INSERT INTO `itemboot` (`label`, `kernel`, `append`) VALUES
('1', 'KERNEL syslinux/chain.c32', 'APPEND hd0'),
('1_localboot', 'LOCALBOOT 0', ' '),
('11', 'KERNEL syslinux/chain.c32', 'APPEND hd0 1'),
('12', 'KERNEL syslinux/chain.c32', 'APPEND hd0 2'),
('ogClientUser', 'KERNEL ogclient/ogvmlinuz', 'APPEND initrd=ogclient/oginitrd.img ro boot=oginit vga=788 irqpoll acpi=on og2nd=sqfs ogprotocol=smb ogactiveadmin=false'),
('ogClientAdmin', 'KERNEL ogclient/ogvmlinuz', 'APPEND initrd=ogclient/oginitrd.img ro boot=oginit vga=788 irqpoll acpi=on og2nd=sqfs ogprotocol=smb ogactiveadmin=true'),
('ogInitrdUser', 'KERNEL linux', 'APPEND initrd=initrd.gz ip=dhcp ro vga=788 irqpoll acpi=on boot=user '),
('ogInitrdAdmin', 'KERNEL linux', 'APPEND initrd=initrd.gz ip=dhcp ro vga=788 irqpoll acpi=on boot=admin ');




CREATE TABLE IF NOT EXISTS `menuboot` (
  `label` varchar(50) collate utf8_spanish_ci NOT NULL,
  `prompt` int(11) NOT NULL,
  `timeout` int(30) default NULL,
  `description` varchar(50) collate utf8_spanish_ci NOT NULL,
  PRIMARY KEY  (`label`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;



INSERT INTO `menuboot` (`label`, `prompt`, `timeout`, `description`) VALUES
('1', 0, 10, 'mbr 1hd'),
('11', 0, 10, '1hd 1particion'),
('12', 0, 10, '1hd 2particion'),
('pxe', 0, 10, 'og client - user'),
('pxeADMIN', 0, 10, 'OgClient - admin');


CREATE TABLE IF NOT EXISTS `menuboot_itemboot` (
  `labelmenu` varchar(100) NOT NULL,
  `labelitem` varchar(100) NOT NULL,
  `default` tinyint(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


INSERT INTO `menuboot_itemboot` (`labelmenu`, `labelitem`, `default`) VALUES
('0', '0', 0),
('11', '11', 0),
('12', '12', 0),
('1', '1', 0),
('pxe', 'ogClientUser', 0),
('pxeADMIN', 'ogClientAdmin', 0);

