<?php
// ������������ ����� �������� (attendee) ����� ������ ����������� ���� ��������, �������� ����� ������� � �.�

class NLMailCounterEvent extends NLMailVEvent {
	
	function NLMailCounterEvent() {
		$this->_method = 'COUNTER';
		
		$this->_requiredProps = array(
			'DTStamp',
			'DTStart',
			'organizer',
			'summary',
			'uid'
		);
		
		$this->_uselessProps = array();
		
		$this->NLMailVEvent();
	}
}
?>