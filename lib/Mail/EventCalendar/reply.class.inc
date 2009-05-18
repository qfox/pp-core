<?php
// �������� (attendee) �������� �� request �������

class NLMailReplyEvent extends NLMailVEvent {
	
	function NLMailReplyEvent() {
		$this->_method = 'REPLY';
		
		$this->_requiredProps = array(
			'attendee',
			'DTStamp',
			'organizer',
			'uid'
		);
		
		$this->_uselessProps = array('alarm');
		
		$this->NLMailVEvent();
	}
}
?>