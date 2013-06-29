<?php
class PXCacheRedis implements IPXCache {
	protected $impl = null;
	protected $host = "localhost";
	protected $port = 6379;
	protected $database = 0;
	protected $cacheDomain = null;
	
	public function __construct($cacheDomain = null, $defaultExpire = 3600, $connectorArgs = null) {
		extension_loaded("redis") or FatalError(get_class($this) . " error: redis extension doesn't loaded or installed");

		$this->impl = new Redis();
		$this->host = empty($connectorArgs['host']) ? $this->host : $connectorArgs['host'];
		$this->port = empty($connectorArgs['port']) ? $this->port : intval($connectorArgs['port']);
		$this->database = empty($connectorArgs['path']) ? $this->database : intval(ltrim($connectorArgs['path'], '/'));
		$this->cacheDomain = $cacheDomain;
		$this->connect();
	}
	
	private function connect() {
		try {
			$this->impl->connect($this->host, $this->port, 1);
			$this->impl->setOption(Redis::OPT_SERIALIZER, Redis::SERIALIZER_PHP);
			if (!empty($this->cacheDomain)) {
				$this->impl->setOption(Redis::OPT_PREFIX, $this->cacheDomain.':');
			}
			$this->impl->select($this->database);
		} catch (Exception $e) {
			trigger_error("Can't connect to Redis: {$this->host}:{$this->port}");
			$this->impl = null;
		}
	}

	private function key($key) {
		return md5($key);
	}
	
	public function exists($key) {
		if(!$this->impl) return false;
		return $this->impl->exists($this->key($key));
	}
	
	public function save($key, $data, $expTime = null) {
		if(!$this->impl) return false;
		return $this->impl->set($this->key($key), $data, $expTime);
	}
	
	public function load($key) {
		if(!$this->impl) return null;
		$data = $this->impl->get($this->key($key));
		$data = ($data === false) ? null : $data;
		return $data;
	}
	
	public function delete($key) {
		if(!$this->impl) return false;
		return $this->impl->delete($this->key($key));
	}
	
	public function clear() {
		if(!$this->impl) return false;
		return $this->impl->flushDB();
	}
}
?>