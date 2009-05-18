<?php
class PXErrorReporterHTML extends PXErrorReporter {
	function PXErrorReporterHTML() {
		$this->computer = IS_WIN && isset($_ENV['COMPUTERNAME']) ? strtolower($_ENV['COMPUTERNAME']) : @rtrim(`hostname`);

		$this->toEmail = isset($_SERVER['SERVER_ADMIN']) ? $_SERVER['SERVER_ADMIN'] : 'support@dalee.ru';
		$this->fromEmail = 'robot@'.$this->computer;
	}

	function warning($error) {
		// pass
	}

	function fatalWarning($error) {
		echo <<<HTML
			<html>
				<head>
					<title>Error</title>
					<style type="text/css">
						body {
							background-color: #ffffff;
							font-family: Lucida Consolas, monospace;
							color: #000000;
						}
					</style>
				</head>

				<body>
					<h1>Error</h1>
					<p>Sorry, but fatal error occured and your request can not be contented</p>
					<p>Please, inform <a href="mailto:{$this->toEmail}">site administrator</a> about it.</p>

					<h1>������</h1>
					<p>��������, ��������� ������, ��� ������ �� ����� ���� ��������� ��������.</p>
					<p>����������, �������� � ��� <a href="mailto:{$this->toEmail}">�������������� �����</a>.</p>
				</body>
			</html>
HTML;
	}

	function _getMailSubject() {
		$errorTypes = array();
		foreach($this->errors as $e) {
			@$errorTypes[$e['type']]++;
		}

		uksort(
			$errorTypes,
			create_function(
				"\$b, \$a",
				<<<SORT_FUNCTION
					if(\$a == \$b) {
						return 0;
					}

					if(\$a == 'Fatal Error') {
						return 1;
					}

					if(\$b == 'Fatal Error') {
						return -1;
					}

					if(\$a == 'Notice') {
						return -1;
					}

					if(\$b == 'Notice') {
						return 1;
					}

					return 1;
SORT_FUNCTION
			)
		);

		$subject = array();
		foreach($errorTypes as $type=>$count) {
			$subject[] = $type .'('.$count.')';
		}

		return $this->computer.': '.implode(', ', $subject);
	}

	function sendReport() {
		$subject = $this->_getMailSubject();

		$mail = new NLMailMessage();
		$this->mail =& $mail;

		$mail->setFrom('Error Reporter', $this->fromEmail);
		$mail->setSubject($subject);
		$mail->setTo($this->toEmail, $this->toEmail);
		$mail->setFormat('html');
		$mail->setCharset('utf-8');

		$errorsListHTML = array();
		foreach($this->errors as $errPos=>$error) {
			$cls = strtolower(str_replace(' ', '', $error['type']));
			$this->attachError($error, $errPos);

			$errorsListHTML[] = <<<HTML
				<li class="{$cls}">
					<strong>{$error['type']}</strong>:
					{$error['message']}
				</li>
HTML;
		}

		$errorsListHTML = implode('', $errorsListHTML);
		$environmentsUris = $this->environmentsUris();

		$body = <<<HTML
			<html>
				<head>
					<title>{$subject}</title>

					<style type="text/css">
						body {
							background-color: #ffffff;
							font-family: Lucida Consolas, monospace;
							color: #000000;
						}

						dl {
							overflow: hidden;
						}

						dt, dd {
							float: left;
							display: block;
							padding: 0;
							margin: 0;
						}

						dt {
							width: 10em;
							clear: left;
						}

						dd {
						}

						li.fatalerror { color: #ff0000; }
						li.notice     { color: #999999; }
					</style>
				</head>

				<body>
					<ol>
						{$errorsListHTML}
					</ol>

					<dl>
						{$environmentsUris}
					</dl>
				</body>
			</html>
HTML;

		$this->addEnvironments();
		$this->addXSLT();

		$mail->setBody(myconv('k', 'u', $body));
		$mail->send();
	}

	function environmentsUris() {
		$requestUri  = 'http://'.NLRequest::GetHttpHost().urldecode(NLRequest::GetRequestUri());
		$remoteAddr  = NLRequest::GetRemoteAddr();
		$httpReferer = urldecode(NLRequest::GetHttpReferer());

		return <<<HTML
			<dt>RequestUri:</dt>
			<dd>
				<a target="_blank" href="{$requestUri}">{$requestUri}</a>
			</dd>

			<dt>RemoteAddr:</dt>
			<dd>
				{$remoteAddr} <a target="_blank" href="http://www.nic.ru/whois/?ip={$remoteAddr}">whois</a>
			</dd>

			<dt>HttpReferer:</dt>
			<dd>
				<a target="_blank" href="{$httpReferer}">{$httpReferer}</a>
			</dd>
HTML;
	}

	function addXSLT() {
		$this->mail->addFile(dirname(__FILE__).'/style.xsl', 'error_reporter.xsl', 'text/xsl');
	}

	function addEnvironments() {
		$xml   = array('<?xml version="1.0" encoding="UTF-8"?>');
		$xml[] = '<?xml-stylesheet type="text/xsl" href="error_reporter.xsl"?>';
		$xml[] = '<environments>';

		$attach = array('_POST'=>NULL, '_GET'=>NULL, '_COOKIE'=>NULL, '_SESSION'=>NULL, '_SERVER'=>NULL, '_FILES'=>NULL);

		foreach($attach as $k=>$v) {
			if(!isset($GLOBALS[$k]) || !is_array($GLOBALS[$k]) || !sizeof($GLOBALS[$k])) {
				continue;
			}

			$xml[] = "\t<{$k}>";

			foreach($GLOBALS[$k] as $kk=>$vv) {
				$node = urldecode($kk);
				$xml[] = "\t\t".'<'.$node.'><![CDATA['.urldecode(trim(d3($vv,   false))).']]></'.$node.'>';
			}

			$xml[] = "\t</{$k}>";
		}

		$xml[] = '</environments>';

		$this->mail->addFile(null, 'environments.debug_xml', 'text/xml', implode("\n", $xml));
	}

	function attachError($error, $errPos) {
		$this->mail->addFile(null, 'error_'.($errPos + 1).'.debug_xml', 'text/xml', myconv('k', 'u', $this->errorToXML($error)));
	}

	function errorToXML($error) {
		$traceXML = $this->traceToXML($error['trace']);

		return <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="error_reporter.xsl"?>
<error>
	<type>{$error['type']}</type>
	<code>{$error['code']}</code>
	<file>{$error['file']}</file>
	<line>{$error['line']}</line>
	<message>
		<![CDATA[
			{$error['message']}
		]]>
	</message>

	<trace>{$traceXML}
	</trace>
</error>
XML;
	}

	function traceToXML($trace) {
		return implode(
			"\n", 

			array_map(
				create_function(
					'$error',

					<<<TRACE_TO_XML
						\$arguments = implode(
							'',

							array_map(
								create_function(
									'\$a', 
									" return \"\n\t\t\t\t\".'<'.\\\$a['type'].'>'.\\\$a['value'].'</'.\\\$a['type'].'>';"
								),

								\$error['args']
							)
						);

						return "
		<call>
			<file>{\$error['file']}</file>
			<line>{\$error['line']}</line>
			<method>{\$error['function']}</method>
			<arguments>{\$arguments}
			</arguments>
		</call>";
TRACE_TO_XML
				),

				$trace
			)
		);
	}
}
?>