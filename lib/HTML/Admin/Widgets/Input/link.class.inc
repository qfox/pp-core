<?php
class PXInputLinkToFile extends PXAdminWidget {
	var $name;
	var $value;

	function PXInputLinkToFile($name, $value) {
		$this->name = $name;
		$this->value = $value;
	}

	function html() {
		$lastDir = isset($_COOKIE['fileManagerDir']) && strlen(trim($_COOKIE['fileManagerDir'])) ? 'htdocs/'.trim($_COOKIE['fileManagerDir']) : '';

		$html  = '<div class="imagepreview">';
		$html .= '<input type="hidden" name="'.$this->name.'[filename]" id="'.$this->name.'_filename" value="'.$this->value['filename'].'">';
		$html .= '<input type="hidden" name="'.$this->name.'[dir]"      id="'.$this->name.'_dir"      value="'.$this->value['dir'].'">';

		$html .= '<input id="'.$this->name.'_fullpath" type="text" value="'.(is_null($this->value['filename']) ? '' : $this->value['dir'].$this->value['filename']).'" readonly/>';
		$html .= '<span>����</span>';

		$html .= '<input id="'.$this->name.'_size"     type="text" value="'.(is_null($this->value['filename']) ? '' : $this->value['size']).'"                       readonly name="'.$this->name.'[size]" />';
		$html .= '<span>������</span>';

		$html .= '<button onclick="EditLinkToFile(\''.$this->name.'\', (this.form.'.$this->name.'_dir.value.length ? \'htdocs\'+this.form.'.$this->name.'_dir.value : \''.$lastDir.'\')); return false;" '.$this->disable.'>��������</button>';

		if (!is_null($this->value['filename'])) {
			$html .= '<br>';
			$html .= '<button onclick="javascript: window.open(this.form.'.$this->name.'_fullpath.value)">��������</button>';
			$html .= '<button onclick="return UnLinkFile(\''.$this->name.'\', 1);"                             >��������</button>';
		}

		return $html;
	}
}
?>