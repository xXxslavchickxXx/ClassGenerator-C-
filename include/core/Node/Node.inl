namespace cg::src {
	template<typename T>
	T* Node::as() {
		return dynamic_cast<T*>(this);
	}

	template<typename T>
	const T* Node::as() const {
		return dynamic_cast<T*>(this);
	}
}