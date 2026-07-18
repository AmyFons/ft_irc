class Client {
	public :
		Client() { _name = "Jean"; _id = 42;}
		~Client() {}

		std::string getName() const {return _name;}
	private:
		std::string _name;
		int _id;
};