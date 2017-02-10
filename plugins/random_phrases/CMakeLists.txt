include(ExternalProject)
add_custom_target(
		random_phrases.py ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/random_phrases.py ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT Copying random_phrases.py
)
add_custom_target(
		random_phrases.cmd ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/random_phrases.cmd ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT Copying random_phrases.cmd
)
add_custom_target(
		phrases.json ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/phrases.json ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT Copying phrases.json
)
