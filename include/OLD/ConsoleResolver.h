#pragma once

namespace cg::validate {
	inline Validator::Validator() {
		cr = pl::error::ConflictResolver<CGError, CGContext>(
			// Скобки которые объявляют список нод анордеред карты
			// Тут будут дефолтные для консоли резолверы
			{
				// Тут идет еденица анордеред карты
				// ключ: тип ошибки
				// значение: лямбда
				{
					CGError::TypeNotTemplate, [](const auto& ctx) {
						std::cout
						<< "1. toggle to template\n"
						<< "2. ignore (Danger - can throw extension)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::VariadicInArgsDoNotLast, [](const auto& ctx) {
						std::cout
						<< "1. swap to end\n"
						<< "2. toggle to non variadic\n"
						<< "3. ignore (Danger - can throw extension)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::FieldIsVariadic, [](const auto& ctx) {
						std::cout
						<< "1. toggle to non variadic\n"
						<< "2. ignore (Danger - the generated code may not be compilable)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::MissMatchVariadic, [](const auto& ctx) {
						std::cout
						<< "1. toggle declare parameter variadic status\n"
						<< "2. ignore (Danger - the generated code may not be compilable)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::VariadicDoNotOnTheEnd, [](const auto& ctx) {
						std::cout
						<< "1. move to the end\n"
						<< "2. toggle to non varadic\n"
						<< "3. ignore (Danger - can throw extension)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::ApplyOptionB;
								case '3':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::MuchVariadicTemplates, [](const auto& ctx) {
						std::cout
						<< "1. toggle first to non variadic\n"
						<< "2. toggle second to non variadic\n"
						<< "3. ignore (Danger - can throw extension)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::ApplyOptionB;
								case '3':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::TemplateTypeIsNotLinked_Local, [](const auto& ctx) {
						std::cout
						<< "1. link to local\n"
						<< "2. ignore (Danger - the generated code may not be compilable)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::FieldMutableAndStatic, [](const auto& ctx) {
						std::cout
						<< "1. toggle to unmutable\n"
						<< "2. toggle to unstatic\n"
						<< "3. ignore (Danger - the generated code may not be compilable)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::ApplyOptionB;
								case '3':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::TypeShouldNotBeTemplate, [](const auto& ctx) {
						std::cout
						<< "1. toggle to typename\n"
						<< "2. ignore\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				},
				{
					CGError::TemplateTypeIsNotLinked_External, [](const auto& ctx) {
						std::cout
						<< "1. link to local\n"
						<< "2. link to external\n"
						<< "3. ignore (Danger - the generated code may not be compilable)\n";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case '1':
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case '2':
									return pl::error::ResolutionStrategy::ApplyOptionB;
								case '3':
									return pl::error::ResolutionStrategy::Ignore;
								default:
									std::cout << "incorrect choise! try again\n";
									break;
							}
						}
					}
				}
			}
		);
	}
}