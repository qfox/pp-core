<?php
class PXAbstractPlugin {
	private $name = null;

	function __construct($app, $name) {
		$this->name = $name;
		$this->registerComponents($app);
	}

	function initSet($params = null) {
	}

	private function registerComponents($app) {
		$fileName = BASEPATH . 'libpp/plugins/' . $this->name . '/config.xml';
		if(!file_exists($fileName)) {
			return;
		}

		if (!($dom = PXml::load($fileName))) {
			FatalError("������ � ����� �������� <em>$fileName</em>");
		}

		$domComs = $dom->xpath("/components//component");
		if (!is_array($domComs) || count($domComs) == 0) {
			FatalError("�� ������� ���������� �������");
		}

		foreach($domComs as $domCom) {
			$attrs = $domCom->attributes();
			foreach($attrs as $item) {
				${$item->name} = utf8_decode($item->value);
			}
			if(!isset($type)) {
				FatelError("������� ��� ����������");
			}
			$type = 'register' . ucfirst($type);
			$this->$type($domCom->childNodes(), $app);
		}
	}

	private function registerTrigger($nodes, $app) {
		$args = array();
		foreach($nodes as $k => $node) {
			$attrs = $node->attributes();
			foreach($attrs as $attr) {
				$args[$attr->name] = $attr->value;
			}
		}
		if(!isset($args['type'])) {
			FatalError('������� ��� trigger');
		}
		$type = $args['type'];
		unset($args['type']);
		if(empty($args)) {
			FatalError('������� ��� trigger');
		}
		$args['folder'] = $this->name;

		$app->registerTrigger($type, $args);
	}

}
?>