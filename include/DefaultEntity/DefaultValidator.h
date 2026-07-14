#pragma once

#include <ErrorEvents.h>
#include <iostream>

namespace cg::validate {
	enum class TEGError {
		Ignore,
		TypeNotTemplate
	};
	class TemplateEntityValidator {
		static pl::error::ConflictResolver<TEGError> cr;

	public:
		static pl::error::ConflictResolver<TEGError>& get_conflict_resolver();

	};

	inline pl::error::ConflictResolver<TEGError> TemplateEntityValidator::cr =
		pl::error::ConflictResolver<TEGError>(
			{
				{
					TEGError::TypeNotTemplate, [](const auto& ctx) {
						std::cout
						<< "1. toggle to template\n"
						<< "2. ignore";

						while (true) {
							char c;
							std::cin >> c;
							switch (c) {
								case 1:
									return pl::error::ResolutionStrategy::ApplyOptionA;
								case 2:
									return pl::error::ResolutionStrategy::Ignore;
							}
						}
					}
				}
			});

	inline pl::error::ConflictResolver<TEGError>& TemplateEntityValidator::get_conflict_resolver() {
		return cr;
	}
}