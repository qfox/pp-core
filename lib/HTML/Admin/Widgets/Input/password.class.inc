<?php
class PXInputPassword extends PXAdminWidget {
	var $value;
	var $name;
	var $canDelete = true;

	function PXInputPassword($name, $value) {
		$this->value = $value;
		$this->name = $name;
	}

	function canDelete($canDelete) {
		$this->canDelete = $canDelete;
	}

	function html() {
		$name = $this->name;

		if(strlen($this->value) == 32 && $this->canDelete) {
			$deleteHTML = <<<HTML
				<input type="checkbox" name="{$name}[delete]" id="{$name}[delete]" class="checkbox">
				<label for="{$name}[delete]">������� ������</label>
HTML;
		} else {
			$deleteHTML = '';
		}

		return <<<HTML
			<div class="password">
				<input type="password" name="{$name}[type]"   onfocus="startCheckPassword(this, '{$name}')">
				<input type="password" name="{$name}[retype]" onfocus="startCheckPassword(this, '{$name}')">

				{$deleteHTML}

				<span>����� ������ ������ ���� �� ����� 6 ��������. ������� ������ ������ �������� � ���� �������� � ��������� ����� ���������� ��������, � ����� �����.</span>
			</div>
HTML;
	}
}
?>