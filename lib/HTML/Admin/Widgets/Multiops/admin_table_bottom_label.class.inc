<?php
	class PXAdminTableBottomLabel extends PXAdminWidget {
		function html() {
			$objectType = $this->parentWidget->dtype->id;
			return <<<HTML
				<div class="multiple-bottom-controls">
					�������� � �����������: 
				</div>
				<div dtype="{$objectType}" class="display-none"></div>
HTML;
		}
	}
?>