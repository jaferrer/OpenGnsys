<?php
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: incorporaordenadores.php
// Descripción : 
//		Da de alta en la base de datos nuevos ordenadores desde un fichero de configuración dhcp
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/CreaComando.php");
include_once("../includes/tftputils.php");
include_once("../idiomas/php/".$idioma."/incorporaordenadores_".$idioma.".php");
include_once("../idiomas/php/".$idioma."/avisos_".$idioma.".php");
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexión con servidor B.D.
//___________________________________________________________________________________________________
$swf=0; 
$idaula=0; 
$nombreaula="";
$contenido="";

if (isset($_GET["idaula"])) $idaula=$_GET["idaula"]; 
if (isset($_GET["nombreaula"])) $nombreaula=$_GET["nombreaula"]; 

if (isset($_POST["swf"])) $swf=$_POST["swf"]; // Recoge parametros
if (isset($_POST["contenido"])) $contenido=$_POST["contenido"]; 
if (isset($_POST["idaula"])) $idaula=$_POST["idaula"]; 
if (isset($_POST["nombreaula"])) $nombreaula=$_POST["nombreaula"]; 

$resul=0;
$ordDup="";

if(!empty($contenido)){ // Se ha introducido contenido en lugar de fichero
	$resul=procesaLineas($cmd,$idaula,$contenido);
}
//___________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<LINK rel="stylesheet" type="text/css" href="../estilos.css">
</HEAD>
<BODY>
<FORM action="incorporaordenadores.php" method="post" name="fdatos">
	<INPUT type=hidden name=swf value=1>
	<INPUT type=hidden name=idaula value=<?php echo $idaula?>>
	<INPUT type=hidden name=nombreaula value=<?php echo $nombreaula?>>
	<BR>
	<P align=center class=cabeceras><?php echo $TbMsg[0]?><BR>
	<SPAN align=center class=subcabeceras><IMG src="../images/iconos/aula.gif">&nbsp;<?php echo $TbMsg[1].":".$nombreaula ?></SPAN></P>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
<table align="center" class="tabla_datos" border="0" cellpadding="0" cellspacing="1">
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	<tr> 
	    <th>&nbsp;<?php echo $TbMsg["MSG_DHCPCODE"]?>&nbsp;<br>
		&nbsp;<?php echo $TbMsg["MSG_DHCPHOST"]?>&nbsp;</th>
	    <td><textarea class="cajatexto" name="contenido" cols="70" rows="18"></textarea></td></tr>
	<tr><th colspan="2">&nbsp;<?php echo $TbMsg["WARN_NAMELENGTH"]?>&nbsp;</th></tr>
	<tr><th colspan="2">&nbsp;<?php echo $TbMsg["WARN_NETBOOT"]?>&nbsp;</th></tr>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
</table>
 </FORM>
<TABLE align=center>
	<TR>
		<TD><IMG src="../images/boton_cancelar.gif" style="cursor:hand"  onclick=""></TD>
		<TD width=20></TD>
		<TD><IMG src="../images/boton_confirmar.gif" style="cursor:hand"  onclick="javascript:document.fdatos.submit();"></TD>
	</TR>
</TABLE>
<?php
//________________________________________________________________________________________________________
// Mensaje con el resultado del proceso
echo '<SCRIPT LANGUAGE="javascript">';
if (!empty($resul))
	echo "	alert('".$TbMsg[$resul]."');";
if (!empty($ordDup))
	echo "	alert('".$TbMsg[5]."\\n".$ordDup."');";
echo '</SCRIPT>';
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?php
// *************************************************************************************************************************************************
function procesaLineas($cmd,$idaula,$buffer)
{
	$nombre="";
	$MAC="";
	$IP="";
	$sw=false;

	$equipos = preg_split('/}/',$buffer);

	// Recorro todos los equipos
	foreach ($equipos as $equipo) {
	    $nombre = strstr($equipo, '{', true);
	    $nombre = str_replace('host', '', $nombre);
	    $valores = strstr($equipo, '{');
	    // Eliminamos caracteres inútiles
	    $valores = str_replace(array (' ','{',':'), '',$valores);

	    $propiedades = preg_split('/;/',$valores);

	    // Recorro todas las propiedades
	    foreach ($propiedades as $propiedad) {
		if (strpos (" $propiedad " , "fixed-address")) {
		    $IP = str_replace("fixed-address", '', $propiedad);
		}
		if (strpos ( " $propiedad " , "hardwareethernet")) {
		    $MAC = str_replace("hardwareethernet", '', $propiedad);
		}
	    }

	    // Si tengo los valores necesario incluyo el equipo
	    if(!empty($nombre) && !empty($MAC) && !empty($IP)){
		if(!Inserta($cmd,$idaula,$nombre,$MAC,$IP)) {
			return(4);
		}
		$sw=true;
		$nombre="";
		$MAC="";
		$IP="";
		$resul=true;
	    }
	}
	if($sw)
	    return(3);
	else
	    return(4);
}
//________________________________________________________________________________________________________
function Inserta($cmd,$idaula,$nombre,$lamac,$laip)
{
	global $ordDup;
	global $idioma;
	
	$grupoid=0;
	$nombreordenador=trim($nombre);
	$ip=trim($laip);
	$auxmac=trim($lamac);
	$mac="";
	for($i=0;$i<strlen($auxmac);$i++)
		if(substr($auxmac,$i,1)!=":")
			$mac.=substr($auxmac,$i,1);
	if(existeOrdenador($cmd,$nombreordenador,$mac,$ip)){
		$ordDup.="Nombre=".$nombre.",Mac=".$mac.",Dirección ip=".$ip." \\n";
		return(true);	
	}

	$idperfilhard=0;
## ADV: modificacion para asignar a los ordenadores, cuando se crean desde "incorpoar ordenadores" el repositorio "default"
	$idrepositorio=1;
	$idconfiguracion=0;
	$cmd->CreaParametro("@grupoid",$grupoid,1);
	$cmd->CreaParametro("@idaula",$idaula,1);
	$cmd->CreaParametro("@nombreordenador",$nombreordenador,0);
	$cmd->CreaParametro("@ip",$ip,0);
	$cmd->CreaParametro("@mac",$mac,0);
	$cmd->CreaParametro("@idperfilhard",$idperfilhard,1);
	$cmd->CreaParametro("@idrepositorio",$idrepositorio,1);
	$cmd->CreaParametro("@idconfiguracion",$idconfiguracion,1);

	$cmd->texto="INSERT INTO ordenadores (nombreordenador, ip, mac, idperfilhard,
				 idrepositorio, router, mascara, idaula, grupoid)
			  SELECT @nombreordenador, @ip, @mac, @idperfilhard,
				 @idrepositorio, router, netmask, @idaula, @grupoid
			    FROM aulas
			   WHERE idaula=".$idaula;
	$resul=$cmd->Ejecutar();
	
	// Crear fichero de arranque PXE con plantilla por defecto.
	if ($resul) {
		$idordenador=$cmd->Autonumerico();
		createBootMode ($cmd, "", $idordenador, $idioma);
	}
	return($resul);
}
//________________________________________________________________________________________________________
//	Recupera los datos de un ordenador
//		Parametros: 
//		- cmd: Una comando ya operativo (con conexión abierta)  
//		- ip: Dirección IP
//________________________________________________________________________________________________________
function existeOrdenador($cmd,$nombre,$MAC,$IP){
	$rs=new Recordset; 
	$cmd->texto="SELECT * FROM ordenadores WHERE nombreordenador='".$nombre."' OR mac='".$MAC."' OR ip='".$IP."'";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	$rs->Primero(); 
	if (!$rs->EOF){
		$rs->Cerrar();
		return(true);
	}
	else
		return(false);
}
?>
