<?php
	// raise test errors
	trigger_error('����� �� ������ � ������� �� �������', E_USER_WARNING);

	d2();
	d();
	$a += 1;

	d2($php_errormsg);

	FatalError('test error');

?>