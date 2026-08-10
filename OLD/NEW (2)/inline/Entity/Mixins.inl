namespace cg::source {
	template<typename T>
	std::shared_ptr<T>
		FabricEntity<T>::create(const std::string& name) {

		return std::make_shared<T>(name);
	}
}