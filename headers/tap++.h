#include <iostream>
#include <string>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/utility/enable_if.hpp>
#include <cmath>

namespace TAP {
	namespace details {
		struct skip_all_type {};
		struct no_plan_type {};
		extern std::ostream* output;
		extern std::ostream* error;
	}
	extern const details::skip_all_type skip_all;
	extern const details::no_plan_type no_plan;
	void plan(int) throw();
	void plan(const details::skip_all_type&, const std::string& = "") throw();
	void plan(const details::no_plan_type&) throw();

	int planned() throw();
	int encountered() throw();

	bool ok(bool, const std::string& = "") throw();
	bool not_ok(bool, const std::string& = "") throw();

	bool pass(const std::string& = "") throw();
	bool fail(const std::string& = "") throw();

	void skip(int, const std::string& = "") throw();
	void bail_out(const std::string& reason) throw();

	int exit_status() throw();
	bool summary() throw();

	void set_output(std::ostream&) throw();
	void set_error(std::ostream&) throw();

	template<typename T> void diag(const T& first) throw() {
		*details::error << "# " << first << std::endl;
	}
	template<typename T1, typename T2> void diag(const T1& first, const T2& second) throw() {
		*details::error << "# " << first << second << std::endl;
	}
	template<typename T1, typename T2, typename T3> void diag(const T1& first, const T2& second, const T3& third) throw() {
		*details::error << "# " << first << second << third << std::endl;
	}
	template<typename T1, typename T2, typename T3, typename T4> void diag(const T1& first, const T2& second, const T3& third, const T4& fourth) throw() {
		*details::error << "# " << first << second << third << fourth << std::endl;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5> void diag(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) throw() {
		*details::error << "# " << first << second << third << fourth << fifth << std::endl;
	}

	template<typename T> void note(const T& first) throw() {
		*details::output << "# " << first << std::endl;
	}
	template<typename T1, typename T2> void note(const T1& first, const T2& second) throw() {
		*details::output << "# " << first << second << std::endl;
	}
	template<typename T1, typename T2, typename T3> void note(const T1& first, const T2& second, const T3& third) throw() {
		*details::output << "# " << first << second << third << std::endl;
	}
	template<typename T1, typename T2, typename T3, typename T4> void note(const T1& first, const T2& second, const T3& third, const T4& fourth) throw() {
		*details::output << "# " << first << second << third << fourth << std::endl;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5> void note(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) throw() {
		*details::output << "# " << first << second << third << fourth << fifth << std::endl;
	}

	template<typename T, typename U> typename boost::disable_if<typename boost::is_floating_point<U>::type, bool>::type is(const T& left, const U& right, const std::string& message = "") {
		try {
			bool ret = ok(left == right, message);
			if (!ret) {
				note("Failed test '", message, "'");
				note("       Got: ", left);
				note("  Expected: ", right);
			}
			return ret;
		}
		catch(const std::exception& e) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought exception '", e.what(), "'");
			note("       Got: ", left);
			note("  Expected: ", right);
			return false;
		}
		catch(...) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought unknown exception");
			note("       Got: ", left);
			note("  Expected: ", right);
			return false;
		}
	}

	template<typename T, typename U> typename boost::disable_if<typename boost::is_floating_point<U>::type, bool>::type isnt(const T& left, const U& right, const std::string& message = "") {
		try {
			return ok(left != right, message);
		}
		catch(const std::exception& e) {
			fail(message);
			note("In test ", message);
			note("Cought exception: ", e.what());
			return false;
		}
		catch(...) {
			fail(message);
			note("In test ", message);
			note("Cought unknown exception");
			return false;
		}
	}

	template<typename T, typename U> typename boost::enable_if<typename boost::is_floating_point<U>::type, bool>::type is(const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
		try {
			bool ret = ok(2 * fabs(left - right) / (fabs(left) + fabs(right)) < epsilon);
			if (!ret) {
				note("Failed test '", message, "'");
				note("       Got: ", left);
				note("  Expected: ", right);
			}
			return ret;
		}
		catch(const std::exception& e) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought exception '", e.what(), "'");
			note("       Got: ", left);
			note("  Expected: ", right);
			return false;
		}
		catch(...) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought unknown exception");
			note("       Got: ", left);
			note("  Expected: ", right);
			return false;
		}
	}

	template<typename T, typename U> typename boost::enable_if<typename boost::is_floating_point<U>::type, bool>::type isnt(const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
		try {
			bool ret = 2 * fabs(left - right) / (fabs(left) + fabs(right)) > epsilon;
			ok(ret, message);
			return ret;
		}
		catch(const std::exception& e) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought exception '", e.what(), "'");
			return false;
		}
		catch(...) {
			fail(message);
			note("Failed test '", message, "'");
			note("Cought unknown exception");
			return false;
		}
	}

	template<typename T, typename U> bool is_convertible(const std::string& message) {
		return ok(boost::is_convertible<T, U>::value, message);
	}

	template<typename T, typename U> bool is_inconvertible(const std::string& message) {
		return ok(!boost::is_convertible<T, U>::value, message);
	}

	std::string todo;
}

#ifdef WANT_TEST_EXTRAS

namespace TAP {
	namespace details {
		struct Skip_exception {
			const std::string reason;
			Skip_exception(const std::string& _reason) : reason(_reason) {
			}
		};
		struct Todo_exception {
			const std::string reason;
			Todo_exception(const std::string& _reason) : reason(_reason) {
			}
		};

		void start_block(int);
		int stop_block();

		class todo_guard {
			const std::string value;
			public:
			todo_guard();
			~todo_guard();
		};
	}

	void skip(const std::string& reason);
	void skip_todo(const std::string& reason);
}

#define TRY(action, name) do {\
		try {\
			action;\
			TAP::pass(name);\
		}\
		catch (const std::exception& e) {\
			TAP::fail(name);\
			note("Caught exception: ", e.what());\
		}\
		catch (...) {\
			TAP::fail(name);\
		}\
	} while (0)

#define FAIL(action, name) do {\
		try {\
			action;\
			TAP::fail(name);\
		}\
		catch (...) {\
			TAP::pass(name);\
		}\
	} while (0)

#define TEST_START(num) {\
		const char* _current_message = NULL;\
		TAP::plan(num);\
		try {

#define TEST_END \
			if (TAP::encountered() < TAP::planned()) {\
				TAP::note("Looks like you planned ", TAP::planned(), " tests but only ran ", TAP::encountered());\
			}\
			else if(TAP::encountered() > TAP::planned()) {\
				TAP::note("Looks like you planned ", TAP::planned(), " tests but ran ", TAP::encountered() - TAP::planned(), " extra");\
			}\
		}\
		catch(TAP::details::Skip_exception& skipper) {\
			TAP::skip(TAP::encountered() - TAP::planned(), skipper.reason);\
		}\
		catch(TAP::details::Todo_exception& todoer) {\
			/*TODO*/\
		}\
		catch(const std::exception& e) {\
			if(_current_message) TAP::fail(_current_message);\
			note("Got error: ", e.what());\
		}\
		catch (...) {\
			if(_current_message) TAP::fail(_current_message);\
		}\
		return TAP::exit_status();\
	}

#define BLOCK_START(planned) \
	try {\
		details::todo_guard foo##planned;\
		TAP::details::start_block(planned);


#define BLOCK_END \
		if (TAP::encountered() != TAP::details::stop_block()) {\
			note("There seems to be a wrong number of tests!");\
		}\
	}\
	catch(TAP::details::Skip_exception& skipper) {\
		TAP::skip(TAP::encountered() - TAP::planned(), skipper.reason);\
	}\
	catch(TAP::details::Todo_exception& todoer) {\
		/*TODO*/\
	}\
	catch(const std::exception& e) {\
		TAP::fail(_current_message);\
		note("Got error: ", e.what());\
	}\
	catch (...) {\
		TAP::fail(_current_message);\
	}

/* This small macro is a main reason for this ugly exercise. I can't introduce a new scope because
 * code subsequent to the declaration should be visible to the rest of the code. At the same time, it
 * must be exception safe. They are quite severe constraints :-(.
 */
#define TRY_DECL(action, new_message) \
	_current_message = new_message;\
	action;\
	TAP::pass(_current_message);\
	_current_message = NULL

#endif /*WANT_TEST_EXTRAS*/