add_custom_target(
		random_phrases.py ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/random_phrases.py ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT Copying random_phrases.py
)
