<?php
	class PXAdminTableChangeParent extends PXAdminWidget {
		function html() {
			$fields = $this->parentWidget->dtype->fields;
			return isset($fields['parent']) || isset($fields['pid']) ? $this->_drawMoveIntoSectionLink() : '';
		}
		
		function _getAllowedSections(){
			$param                       = array('datatype' => $this->parentWidget->dtype);
			list($format, $parentFormat) = PXDisplayTypeParentdropdown::getFormats($param);
			$tree                        = PXDisplayTypeParentdropdown::getParentTree($object = null,$parentFormat);
			$parents                     = $tree->getPlainTree(null);
			
			foreach($parents as $k => $v){
				@$allowed = $tree->leafs[$k]->content['allowed'];
				
				if(isset($allowed[$format]) || $format == $parentFormat){
					$parents[$k] = $v;
				} else {
					unset($parents[$k]);
				}
			}
			
			return array(null => '--- �������� ������ ---') + $parents;
		}
		
		function _drawMoveIntoSectionLink(){
			$opts = '';
			foreach($this->_getAllowedSections() as $id => $name){
				$opts .= "<option value=\"{$id}\"}'>{$name}</option>";
			}
			return <<<HTML
				<div class="multiple-bottom-controls">
					<a href="#doChangeParent" helper="true" confirm="�� ������� ��� ������ ����������� ���������� ������� � ��������� ������?">����������� � ...</a>
					<div class="helper">
						<div class="parent-section-changer">
							<form>
								<select assign='{"parent": \$(this).val()}'>
									{$opts}
								</select>
							</form>
						</div>
					</div>
				</div>
HTML;
		}
	}
?>