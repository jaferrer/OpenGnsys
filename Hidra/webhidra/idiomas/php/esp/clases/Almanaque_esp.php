	<?
/*============================================================================
	Esta clase genera tablas HTML para selecci�n de fechas (Versi�n inglesa)
	
	Atributos de la clase:

		clase: Clase [CSS] de la tabla HTML que se generar�.
		onmouseover: Funci�n Javascript que se ejuctar� al generarse el evento
		onmouseout: Funci�n Javascript que se ejuctar� al generarse el evento
		onclick: Funci�n Javascript que se ejuctar� al hacer click sobre el objeto

============================================================================*/
class Almanaque{

	var $clase ;
	var $onmouseover;
	var $onmouseout;
	var $onclick;

	var $desplazamiento_dias=6; // Ajuste fino dependiendo del a�o de comienzo del algoritmo
	var $nombre_mes=array();
	var $nombre_dia=array();
	var $dias_meses=array();
	var $semanas=array();
	var $numero_annos=array();

	function Almanaque($pclase="",$ponmouseover="sobre(this)",$ponmouseout="fuera(this)",$ponclick="clic(this)"){ //Constructor
		$this->clase=$pclase;
		$this->onmouseover=$ponmouseover;
		$this->onmouseout=$ponmouseout;
		$this->onclick=$ponclick;
		
		$this->nombre_mes[1]=array ("Enero",0x0001); 
		$this->nombre_mes[2]=array ("Febrero",0x0002);
		$this->nombre_mes[3]=array ("Marzo",0x0004);
		$this->nombre_mes[4]=array ("Abril",0x0008);
		$this->nombre_mes[5]=array ("Mayo",0x0010);
		$this->nombre_mes[6]=array ("Junio",0x0020);
		$this->nombre_mes[7]=array ("Julio",0x0040);
		$this->nombre_mes[8]=array ("Agosto",0x0080);
		$this->nombre_mes[9]=array ("Septiembre",0x0100);
		$this->nombre_mes[10]=array ("Octubre",0x0200);
		$this->nombre_mes[11]=array ("Noviembre",0x0400);
		$this->nombre_mes[12]=array ("Diciembre",0x0800);

		$this->nombre_dia[1]=array ("L",0x01); // tama�o 1 bytes
		$this->nombre_dia[2]=array ("M",0x02); 
		$this->nombre_dia[3]=array ("X",0x04); 
		$this->nombre_dia[4]=array ("J",0x08); 
		$this->nombre_dia[5]=array ("V",0x10); 
		$this->nombre_dia[6]=array ("S",0x20); 
		$this->nombre_dia[7]=array ("D",0x40); 

	
		$this->dias_meses[1]=31;
		$this->dias_meses[2]=28;
		$this->dias_meses[3]=31;
		$this->dias_meses[4]=30;
		$this->dias_meses[5]=31;
		$this->dias_meses[6]=30;
		$this->dias_meses[7]=31;
		$this->dias_meses[8]=31;
		$this->dias_meses[9]=30;
		$this->dias_meses[10]=31;
		$this->dias_meses[11]=30;
		$this->dias_meses[12]=31;

		$this->semanas[1]=array ("1�",0x01); // tama�o 1 bytes
		$this->semanas[2]=array ("2�",0x02);
		$this->semanas[3]=array ("3�",0x04);
		$this->semanas[4]=array ("4�",0x08);
		$this->semanas[5]=array ("5�",0x10);
		$this->semanas[6]=array ("�ltima",0x20);

		$this->numero_annos[1]=array ("2004",0x01); // tama�o 1 bytes
		$this->numero_annos[2]=array ("2005",0x02); 
		$this->numero_annos[3]=array ("2006",0x04); 
		$this->numero_annos[4]=array ("2007",0x08); 
		$this->numero_annos[5]=array ("2008",0x10); 
		$this->numero_annos[6]=array ("2009",0x20); 
		$this->numero_annos[7]=array ("2010",0x40); 
		$this->numero_annos[8]=array ("2011",0x80); 

		$this->numero_dias[1]=0x00000001; // tama�o 4 bytes
		$this->numero_dias[2]=0x00000002; 
		$this->numero_dias[3]=0x00000004; 
		$this->numero_dias[4]=0x00000008; 

		$this->numero_dias[5]=0x00000010; 
		$this->numero_dias[6]=0x00000020; 
		$this->numero_dias[7]=0x00000040; 
		$this->numero_dias[8]=0x00000080;

		$this->numero_dias[9]=0x00000100; 
		$this->numero_dias[10]=0x00000200; 
		$this->numero_dias[11]=0x00000400; 
		$this->numero_dias[12]=0x00000800; 

		$this->numero_dias[13]=0x00001000;
		$this->numero_dias[14]=0x00002000;
		$this->numero_dias[15]=0x00004000;
		$this->numero_dias[16]=0x00008000;

		$this->numero_dias[17]=0x00010000;
		$this->numero_dias[18]=0x00020000;
		$this->numero_dias[19]=0x00040000;
		$this->numero_dias[20]=0x00080000;

		$this->numero_dias[21]=0x00100000;
		$this->numero_dias[22]=0x00200000;
		$this->numero_dias[23]=0x00400000;
		$this->numero_dias[24]=0x00800000;
	
		$this->numero_dias[25]=0x01000000;
		$this->numero_dias[26]=0x02000000;
		$this->numero_dias[27]=0x04000000;
		$this->numero_dias[28]=0x08000000;

		$this->numero_dias[29]=0x10000000;
		$this->numero_dias[30]=0x20000000;
		$this->numero_dias[31]=0x40000000;
		$this->numero_dias[32]=0x80000000;



		$this->numero_horas[1]=array	("0:00",	 0x0001);  // tama�o 2 bytes
		$this->numero_horas[2]=array	("1:00",	 0x0002);  
		$this->numero_horas[3]=array	("2:00",	 0x0004);  
		$this->numero_horas[4]=array	("3:00",	 0x0008);  
		$this->numero_horas[5]=array	("4:00",	 0x0010);  
		$this->numero_horas[6]=array	("5:00",	 0x0020);  
		$this->numero_horas[7]=array	("6:00",	 0x0040);  
		$this->numero_horas[8]=array	("7:00",	 0x0080);  
		$this->numero_horas[9]=array	("8:00",	 0x0100);  
		$this->numero_horas[10]=array ("9:00",0x0200);  
		$this->numero_horas[11]=array ("10:00",0x0400);  
		$this->numero_horas[12]=array ("11:00",0x0800);  
	}
	
	/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve el n�mero del d�a de la semana:
			0=domingo 1=Lunes, 2=m�rtes ... 6=s�bado
		
		Par�metro de entrada:
			Una cadena con formato de fecha dd/mm/aaaa.
	________________________________________________________________________________________________________*/
	function _DiaSemana($fecha){
		list($dia,$mes,$anno)=split('[/.-]',$fecha);
		$cont=0;
		for ($i=1900;$i<$anno;$i++){
			if ($this->bisiesto($i)) $dias_anuales=366; else	$dias_anuales=365;
			$cont+=$dias_anuales;
		}
		for ($i=1;$i<$mes;$i++){
			if ($i!=2)
				$cont+=$this->dias_meses[$i];
			else{
				if ($this->bisiesto($anno))
					$cont+=29;
				else
					$cont+=28;
			}
		}
		$cont+=$dia+$this->desplazamiento_dias;
		return($cont%7);
	}
	/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve true si el a�o pasado como par�metro es bisiesto y false si no lo es

		Par�metro de entrada:
			Una n�mero que representa el a�o
	________________________________________________________________________________________________________*/
	function bisiesto($anob){
		if ($anob%4==0) return(true); else return(false);
	}
	/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con un rango de a�os (2003-2010)
	________________________________________________________________________________________________________*/
	function Annos($ano_desde,$ano_hasta){
		$HTML_calendario='<TABLE id="tabla_annos" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH style="cursor:hand" onclick="TH_'.$this->onclick.'">A�os</TH></TR>'.chr(13); // Literal a�os
		for ($i=1;$i<7;$i++){
			$HTML_calendario.='<TR><TD id="'.$this->numero_annos[$i][0].'" value="'.$this->numero_annos[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->numero_annos[$i][0].'</TD></TR>'.chr(13);
		}
		$HTML_calendario.='</TABLE>'.chr(13);
		return($HTML_calendario);
	}	
	/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML del calendario del mes y a�o elegidos
		y que son propiedades de la clase.
	________________________________________________________________________________________________________*/
	function MesAnno($mes,$anno){
		$fecha="1/".$mes."/".$anno;
		$ds=$this->_DiaSemana($fecha);
		if ($ds==0) $ds=7;
		
		$swbi=0; // Suma para bisiesto
		if ($this->bisiesto($anno) && $mes==2)	$swbi=1; 

		$HTML_calendario='<TABLE id="tabla_mesanno" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan=7 style="cursor:hand" onclick="TH_'.$this->onclick.'">'.$this->nombre_mes[$mes][0].'</TH></TR>'.chr(13); // Nombre del mes
		$HTML_calendario.='<TR>'.chr(13);
		for ($i=1;$i<8;$i++)
			$HTML_calendario.='<TH>'.$this->nombre_dia[$i][0].'</TH>'.chr(13); // D�as de la semana
		$HTML_calendario.='</TR><TR>'.chr(13);
		for ($i=1;$i<$ds;$i++)
			$HTML_calendario.='<TD></TD>'.chr(13); // Relleno primeros dias de la semana
		$sm=$ds; // Control salto de semana
		for ($i=1;$i<=$this->dias_meses[$mes]+$swbi;$i++){
			$HTML_calendario.='<TD id="'.$i.'/'.$mes.'/'.$anno.'" value="'.$this->numero_dias[$i].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$i.'</TD>'.chr(13);
			if ($sm%7==0){
				$HTML_calendario.='</TR><TR>'.chr(13);
				$sm=0;
			}
			$sm++;
		}
		$HTML_calendario.='</TR></TABLE>'.chr(13);
		return($HTML_calendario);
	}
	/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con los meses del a�o en dos columnas.
	________________________________________________________________________________________________________*/
	function Meses(){
		$HTML_calendario='<TABLE id="tabla_meses" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan=2 style="cursor:hand" onclick="TH_'.$this->onclick.'">Meses</TH></TR>'.chr(13); // Literal meses
		for ($i=1;$i<13;$i++){
			$HTML_calendario.='<TR><TD id="'.$i.'" value="'.$this->nombre_mes[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->nombre_mes[$i++][0].'</TD>'.chr(13);
			$HTML_calendario.='<TD id="'.$i.'" value="'.$this->nombre_mes[$i][1].'"style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->nombre_mes[$i][0].'</TD></TR>'.chr(13);
		}
		$HTML_calendario.='</TABLE>'.chr(13);
		return($HTML_calendario);
	}
	/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con los d�as de la semana en una fila.
	________________________________________________________________________________________________________*/
	function Dias(){
		$HTML_calendario='<TABLE id="tabla_dias" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH  colspan=7 style="cursor:hand" onclick="TH_'.$this->onclick.'">D�a</TH><TR>'.chr(13); // Literal D�as
		for ($i=1;$i<8;$i++){
			$HTML_calendario.='<TD id="'.$i.'" value="'.$this->nombre_dia[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->nombre_dia[$i][0].'</TD>'.chr(13);
		}
		$HTML_calendario.='</TR></TABLE>'.chr(13);
		return($HTML_calendario);
	}		
	/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con el orden de las semana en una fila.
	________________________________________________________________________________________________________*/
	function Semanas(){
		$HTML_calendario='<TABLE id="tabla_semanas" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH  colspan=7 style="cursor:hand" onclick="TH_'.$this->onclick.'">Semana</TH><TR>'.chr(13); // Literal Semenas
		for ($i=1;$i<7;$i++){
			$HTML_calendario.='<TD id="'.$i.'" value="'.$this->semanas[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->semanas[$i][0].'&nbsp;</TD>'.chr(13);
		}
		$HTML_calendario.='</TR></TABLE>'.chr(13);
		return($HTML_calendario);
	}	
	/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con los 31 d�as de un mes en 3 filas
	________________________________________________________________________________________________________*/
	function DiasMes(){
		$HTML_calendario='<TABLE id="tabla_diasmes" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan=8 style="cursor:hand" onclick="TH_'.$this->onclick.'">D�a de mes</TH><TR>'.chr(13); // Literal D�a
		$HTML_calendario.='<TR>'.chr(13);
		$sd=1; // Control salto de fila
		for ($i=1;$i<32;$i++){
				$HTML_calendario.='<TD id="'.$i.'" value="'.$this->numero_dias[$i].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$i.'</TD>'.chr(13);
				if ($sd%8==0){
					$HTML_calendario.='</TR><TR>'.chr(13);
					$sd=0;
				}
				$sd++;
		}
		$HTML_calendario.='</TR></TABLE>'.chr(13);
		return($HTML_calendario);
	}
	/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con las horas de apertura de las aulas
	________________________________________________________________________________________________________*/
	function Horas(){

		$HTML_calendario='<TABLE  id="tabla_horas" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan=12 style="cursor:hand" onclick="TH_'.$this->onclick.'">Hora ejecuci�n de la acci�n </TH>';
		$HTML_calendario.='<TH>Mod</TH>';
		$HTML_calendario.='<TH>Min.</TH>';
		//$HTML_calendario.='<TH>Seg.</TH></TR>';
		$HTML_calendario.='<TR>'.chr(13);
		for ($i=1;$i<13;$i++)
			$HTML_calendario.='<TD align=center id="'.$this->numero_horas[$i][0].'"  value="'.$this->numero_horas[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->numero_horas[$i][0].'</TD>'.chr(13);

		$HTML_calendario.='<TD align=center>';
		$HTML_calendario.= '<SELECT class="estilodesple" id="ampm">'.chr(13);
		$HTML_calendario.= '<OPTION  value=0>A.M.</OPTION>'.chr(13);
		$HTML_calendario.= '<OPTION selected value=1 >P.M.</OPTION>'.chr(13);
		$HTML_calendario.='</SELECT>'.chr(13);
		$HTML_calendario.='</TD>	'.chr(13);

		$HTML_calendario.='<TD align=center>';
		$HTML_calendario.='<INPUT   type=text class=cajatexto id=minutos size=1>'.chr(13);
		$HTML_calendario.='</TD>	'.chr(13);

		$HTML_calendario.='</TR>'.chr(13);
		$HTML_calendario.='</TABLE>'.chr(13);

		return($HTML_calendario);
	}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con las horas hasta de reserva de las aulas
________________________________________________________________________________________________________*/
	function HorasReserva($literal,$nombretabla,$nombreampm,$nombreminuto){
		if($literal=="1") 
			$literal="Comienzo de la reserva";
		else
			$literal="Fin de la reserva";
		
		$HTML_calendario='<TABLE  id="'.$nombretabla.'" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan=12 style="cursor:hand" onclick="TH_'.$this->onclick.'">'.$literal.' </TH>';
		$HTML_calendario.='<TH>Mod</TH>';
		$HTML_calendario.='<TH>Min.</TH>';
		$HTML_calendario.='<TR>'.chr(13);
		for ($i=1;$i<13;$i++)
			$HTML_calendario.='<TD align=center id="'.$this->numero_horas[$i][0].'"  value="'.$this->numero_horas[$i][1].'" style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$this->numero_horas[$i][0].'</TD>'.chr(13);

		$HTML_calendario.='<TD align=center>';
		$HTML_calendario.= '<SELECT class="estilodesple" id="'.$nombreampm.'">'.chr(13);
		$HTML_calendario.= '<OPTION value=0>A.M.</OPTION>'.chr(13);
		$HTML_calendario.= '<OPTION selected value=1  >P.M.</OPTION>'.chr(13);
		$HTML_calendario.='</SELECT>'.chr(13);
		$HTML_calendario.='</TD>	'.chr(13);

		$HTML_calendario.='<TD align=center>';
		$HTML_calendario.='<INPUT   type=text class=cajatexto id="'.$nombreminuto.'" size=1>'.chr(13);
		$HTML_calendario.='</TD>	'.chr(13);

		$HTML_calendario.='</TR>'.chr(13);
		$HTML_calendario.='</TABLE>'.chr(13);

		return($HTML_calendario);
	}
	/* -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Esta funci�n devuelve una cadena con el c�digo HTML con las horas de apertura de las aulas
	________________________________________________________________________________________________________*/
	function Horas_Completas(){
		$maxcolumnas=16;

		$HTML_calendario='<TABLE id="tabla_horas" class="'.$this->clase.'">'.chr(13);
		$HTML_calendario.='<TR>'.chr(13);
		$HTML_calendario.='<TH colspan='.$maxcolumnas.'>Horas</TH><TR>'.chr(13); // Literal Horas
		$HTML_calendario.='<TR>'.chr(13);
		$currenthora=0;
		$currentminutos=0;
		$currenthorario=$currenthora.":".$currentminutos;
		for ($i=1;$i<97;$i++){
			if($currentminutos==0) $currenthorario.="0";

			$HTML_calendario.='<TD align=center id="'.$currenthorario.'"  style="cursor:hand" onmouseover="'.$this->onmouseover.'" onmouseout="'.$this->onmouseout.'" onclick="'.$this->onclick.'">'.$currenthorario.'</TD>'.chr(13);
			$currentminutos+=15;
			if($currentminutos==60) {
				$currenthora++;
				if($currenthora==24) 	$currenthora=0;
				$currentminutos=0;
			}
			$currenthorario=$currenthora.":".$currentminutos;
			if ($i%$maxcolumnas==0) $HTML_calendario.='</TR><TR>'.chr(13);
		}
	$HTML_calendario.='</TR></TABLE>'.chr(13);
	return($HTML_calendario);
	}
} // Fin de la clase Almanaque