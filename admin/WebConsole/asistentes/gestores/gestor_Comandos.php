<?php
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: gestor_Comandos.php
// Descripción : 
//		Gestor de todos los comandos
// *************************************************************************************************************************************************
include_once("../../includes/ctrlacc.php");
include_once("../../clases/AdoPhp.php");
include_once("../../clases/SockHidra.php");
include_once("../../includes/constantes.php");
include_once("../../includes/comunes.php");
include_once("../../includes/cuestionacciones.php");
include_once("../../includes/CreaComando.php");
include_once("../../includes/RecopilaIpesMacs.php");
//________________________________________________________________________________________________________
include_once("../includes/capturaacciones.php");
//________________________________________________________________________________________________________

 // Recoge parametros de seguimiento
$sw_ejya="";
$sw_seguimiento="";
$sw_ejprg="";

$sw_mkprocedimiento="";
$nombreprocedimiento="";
$idprocedimiento="";
$ordprocedimiento=0;

$sw_mktarea="";
$nombretarea="";
$idtarea="";
$ordtarea=0;

if (isset($_POST["sw_ejya"]))	$sw_ejya=$_POST["sw_ejya"]; 
if (isset($_POST["sw_seguimiento"]))	$sw_seguimiento=$_POST["sw_seguimiento"]; 

if (isset($_POST["sw_ejprg"]))	$sw_ejprg=$_POST["sw_ejprg"]; 

if (isset($_POST["sw_mkprocedimiento"]))	$sw_mkprocedimiento=$_POST["sw_mkprocedimiento"]; 
if (isset($_POST["nombreprocedimiento"]))	$nombreprocedimiento=$_POST["nombreprocedimiento"]; 
if (isset($_POST["idprocedimiento"]))	$idprocedimiento=$_POST["idprocedimiento"]; 
if (isset($_POST["ordprocedimiento"]))	$ordprocedimiento=$_POST["ordprocedimiento"]; 
if(empty($ordprocedimiento)) $ordprocedimiento=0;

if (isset($_POST["sw_mktarea"]))	$sw_mktarea=$_POST["sw_mktarea"]; 
if (isset($_POST["nombretarea"]))	$nombretarea=$_POST["nombretarea"]; 
if (isset($_POST["idtarea"]))	$idtarea=$_POST["idtarea"]; 
if (isset($_POST["ordtarea"]))	$ordtarea=$_POST["ordtarea"]; 
if(empty($ordtarea)) $ordtarea=0;

//__________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexión con servidor B.D.
//__________________________________________________________________
$funcion="nfn=".$funcion.chr(13); // Nombre de la función que procesa el comando y el script que lo implementa
$aplicacion=""; // Ámbito de aplicación (cadena de ipes separadas por ";" y de identificadores de ordenadores por ","
$acciones=""; // Cadena de identificadores de acciones separadas por ";" para seguimiento

$atributos=str_replace('@',chr(13),$atributos); // Reemplaza caracters
$atributos=str_replace('#',chr(10),$atributos); 
$atributos=str_replace('$',chr(9),$atributos);

//__________________________________________________________________
?>
<HTML>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<BODY>
	<SCRIPT language="javascript" src="../../comandos/jscripts/comunescomandos.js"></SCRIPT>
	<?php echo '<SCRIPT language="javascript" src="../../idiomas/javascripts/'.$idioma.'/comandos/comunescomandos_'.$idioma.'.js"></SCRIPT>'?>
<?php

/* Recopila identificadore ,ipes y macs para envío de comandos */
$cadenaid="";
$cadenaip="";
$cadenamac="";
RecopilaIpesMacs($cmd,$ambito,$idambito);

/*--------------------------------------------------------------------------------------------------------------------
	Creación de parametros para sentencias SQL
--------------------------------------------------------------------------------------------------------------------*/
$cmd->CreaParametro("@tipoaccion",0,1);
$cmd->CreaParametro("@idtipoaccion",0,1);
$cmd->CreaParametro("@descriaccion","",0);
$cmd->CreaParametro("@idordenador",0,1);
$cmd->CreaParametro("@ip","",0);
$cmd->CreaParametro("@sesion",0,1);
$cmd->CreaParametro("@parametros","",0);
$cmd->CreaParametro("@fechahorareg","",0);
$cmd->CreaParametro("@estado",0,1);
$cmd->CreaParametro("@resultado",0,1);
$cmd->CreaParametro("@idcentro",0,1);
$cmd->CreaParametro("@idprocedimiento",0,1);
$cmd->CreaParametro("@descripcion","",0);
$cmd->CreaParametro("@idcomando",0,1);
$cmd->CreaParametro("@idtarea",0,1);
$cmd->CreaParametro("@ambito",0,1);
$cmd->CreaParametro("@idambito",0,1);
$cmd->CreaParametro("@restrambito","",0);
$cmd->CreaParametro("@ordprocedimiento",0,1);
$cmd->CreaParametro("@ordtarea",0,1);

if($ambito==0){ // Ambito restringido a un subconjuto de ordenadores con formato (idordenador1,idordenador2,etc)
	$cmd->ParamSetValor("@restrambito",$idambito);
	$idambito=0;
}
$resul=true;
/*--------------------------------------------------------------------------------------------------------------------
	Switch de ejecución inmediata y de seguimiento
--------------------------------------------------------------------------------------------------------------------*/
if($sw_ejya=='on' || $sw_ejprg=="on" ){ 
	$parametros=$funcion.$atributos;
	$aplicacion=chr(13)."ido=".$cadenaid.chr(13)."mac=".$cadenamac.chr(13)."iph=".$cadenaip.chr(13);
	if($sw_seguimiento==1 || $sw_ejprg=="on"){ // Switch de ejecución con seguimiento o comando programado
		$sesion=time();
		$cmd->ParamSetValor("@tipoaccion",$EJECUCION_COMANDO);
		$cmd->ParamSetValor("@idtipoaccion",$idcomando);
		$cmd->ParamSetValor("@descriaccion",$descricomando);
		$cmd->ParamSetValor("@sesion",$sesion);
		$cmd->ParamSetValor("@idcomando",$idcomando);
		$cmd->ParamSetValor("@parametros",$parametros);
		$cmd->ParamSetValor("@fechahorareg",date("y/m/d H:i:s"));
		if($sw_ejprg=="on") // Switch de ejecución con programación (se para el comando tarea para lanzarlo posteriormente)
			$cmd->ParamSetValor("@estado",$ACCION_DETENIDA);
		else
			$cmd->ParamSetValor("@estado",$ACCION_INICIADA);
		$cmd->ParamSetValor("@resultado",$ACCION_SINRESULTADO);
		$cmd->ParamSetValor("@ambito",$ambito);
		$cmd->ParamSetValor("@idambito",$idambito);			
		$cmd->ParamSetValor("@idcentro",$idcentro);
		$auxID=split(",",$cadenaid);
		$auxIP=split(";",$cadenaip);
		for ($i=0;$i<sizeof($auxID);$i++){
			$cmd->ParamSetValor("@idordenador",$auxID[$i]);
			$cmd->ParamSetValor("@ip",$auxIP[$i]);
			$cmd->texto="INSERT INTO acciones (idordenador,tipoaccion,idtipoaccion,descriaccion,ip,
						sesion,idcomando,parametros,fechahorareg,estado,resultado,ambito,idambito,restrambito,idcentro)
						VALUES (@idordenador,@tipoaccion,@idtipoaccion,@descriaccion,@ip,
						@sesion,@idcomando,@parametros,@fechahorareg,@estado,@resultado,@ambito,@idambito,@restrambito,@idcentro)";
			$resul=$cmd->Ejecutar();
			//echo "<br>".$cmd->texto;
		}
		$acciones=chr(13)."ids=".$sesion.chr(13); // Para seguimiento	
	}
	if (!$resul){
		echo '<SCRIPT language="javascript">';
		echo 'resultado_comando(7);'.chr(13);
		echo '</SCRIPT>';
	}
	else{
		if($sw_ejya=='on'){ 	
			// Envio al servidor 
			$shidra=new SockHidra($servidorhidra,$hidraport); 
			if ($shidra->conectar()){ // Se ha establecido la conexión con el servidor hidra
				$parametros.=$aplicacion;
				$parametros.=$acciones;
				//echo $parametros;
				$resul=$shidra->envia_comando($parametros);
				if($resul)
					$trama=$shidra->recibe_respuesta();
					if($resul){
						$hlonprm=hexdec(substr($trama,$LONCABECERA,$LONHEXPRM));
						$parametros=substr($trama,$LONCABECERA+$LONHEXPRM,$hlonprm);
						$ValorParametros=extrae_parametros($parametros,chr(13),'=');
						$resul=$ValorParametros["res"];
					}
				$shidra->desconectar();
			}
			if (!$resul){
				echo '<SCRIPT language="javascript">';
				echo 'resultado_comando(1);'.chr(13);
				echo '</SCRIPT>';
			}
			else{
				echo '<SCRIPT language="javascript">'.chr(13);
				echo 'resultado_comando(2);'.chr(13);
				echo '</SCRIPT>'.chr(13);
			}		
		}
	}
}
/*--------------------------------------------------------------------------------------------------------------------
	Switch de creación o inclusión en procedimiento
--------------------------------------------------------------------------------------------------------------------*/
if($sw_mkprocedimiento=='on' || $sw_mktarea=='on'){ 
	$resul=false;
	if($idprocedimiento==0 || $sw_mktarea=='on'){ // Nuevo procedimiento o Tarea
		if($sw_mktarea=='on' && empty($nombreprocedimiento)){ // Si tarea con inclusión de procedimiento...
			if(!empty($nombretarea))
				$nombreprocedimiento="Proc($nombretarea)";	// .. tarea nueva
			else
				$nombreprocedimiento="Proc($idtarea)";	// .. inclusión en tarea
		}
		$cmd->ParamSetValor("@descripcion",$nombreprocedimiento);
		$cmd->ParamSetValor("@idcentro",$idcentro);
		$cmd->texto="INSERT INTO procedimientos(descripcion,idcentro) VALUES (@descripcion,@idcentro)";
		$resul=$cmd->Ejecutar();
		if($resul){
			if($idprocedimiento==0) // Cambia el identificador sólo si es nuevo procedimiento 
				$idprocedimiento=$cmd->Autonumerico();
			if($sw_mktarea=='on')
				$idprocedimientotarea=$cmd->Autonumerico(); // Identificador para la tarea;	
		}
	}
	if( $idprocedimiento>0 || $sw_mktarea=='on'){ //  inclusión en procedimiento existente 
		$cmd->ParamSetValor("@idprocedimiento",$idprocedimiento);
		$cmd->ParamSetValor("@idcomando",$idcomando);
		$cmd->ParamSetValor("@ordprocedimiento",$ordprocedimiento);
		$parametros=$funcion.$atributos;
		$cmd->ParamSetValor("@parametros",$parametros);
		$cmd->texto="INSERT INTO procedimientos_acciones(idprocedimiento,orden,idcomando,parametros) VALUES (@idprocedimiento,@ordprocedimiento,@idcomando,@parametros)";
		$resul=$cmd->Ejecutar();
		if($sw_mktarea=='on' && $idprocedimiento!=$idprocedimientotarea){ // Si es tarea se graba para su procedimiento independiente aunque los parametros sean los mismos
			$cmd->ParamSetValor("@idprocedimiento",$idprocedimientotarea);		
			$cmd->texto="INSERT INTO procedimientos_acciones(idprocedimiento,orden,idcomando,parametros) VALUES (@idprocedimiento,@ordprocedimiento,@idcomando,@parametros)";
			$resul=$cmd->Ejecutar();
		}
	}
	if (!$resul){
		echo '<SCRIPT language="javascript">';
		echo 'resultado_comando(3);'.chr(13);
		echo '</SCRIPT>';
	}
	else{
		if($sw_mkprocedimiento=='on'){
			echo '<SCRIPT language="javascript">'.chr(13);
			echo 'resultado_comando(4);'.chr(13);
			echo '</SCRIPT>'.chr(13);
		}
	}
}	
/*--------------------------------------------------------------------------------------------------------------------
	Switch de creación o inclusión en tarea 
--------------------------------------------------------------------------------------------------------------------*/
if($sw_mktarea=='on'){ 
	$resul=false;
	if($idtarea==0){ // Nueva tarea
		$cmd->ParamSetValor("@descripcion",$nombretarea);
		$cmd->ParamSetValor("@idcentro",$idcentro);
		$cmd->ParamSetValor("@ambito",$ambito);
		$cmd->ParamSetValor("@idambito",$idambito);		
		$cmd->texto="INSERT INTO tareas(descripcion,idcentro,ambito,idambito,restrambito)
					VALUES (@descripcion,@idcentro,@ambito,@idambito,@restrambito)";
		$resul=$cmd->Ejecutar();
		if($resul)
			$idtarea=$cmd->Autonumerico();
	}
	if($idtarea>0){ //  inclusión en tarea existente 
		$cmd->ParamSetValor("@idtarea",$idtarea);
		$cmd->ParamSetValor("@idprocedimiento",$idprocedimientotarea);
		$cmd->ParamSetValor("@ordtarea",$ordtarea);
		$cmd->texto="INSERT INTO tareas_acciones(idtarea,orden,idprocedimiento) 
							VALUES (@idtarea,@ordtarea,@idprocedimiento)";
		$resul=$cmd->Ejecutar();
		//echo $cmd->texto;
	}
	if (!$resul){
		echo '<SCRIPT language="javascript">'.chr(13);
		echo 'resultado_comando(5);'.chr(13);
		echo '</SCRIPT>'.chr(13);
	}
	else{
		echo '<SCRIPT language="javascript">'.chr(13);
		echo 'resultado_comando(6);'.chr(13);
		echo '</SCRIPT>'.chr(13);
	}
}
/* Programación del comando */
if ($resul){
	if($sw_ejprg=="on" ){ 	
		echo '<SCRIPT language="javascript">'.chr(13);
		echo 'var whref="../../varios/programaciones.php?idcomando='.$idcomando.'";'.chr(13);
		echo 'whref+="&sesion='.$sesion.'&descripcioncomando='.UrlEncode($descricomando).'&tipoaccion='.$EJECUCION_COMANDO.'";'.chr(13);
		echo 'location.href=whref;';
		echo '</SCRIPT>';
	}
}
?>
</BODY>
</HTML>	
