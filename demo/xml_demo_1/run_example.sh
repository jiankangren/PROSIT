#!/bin/sh
CURR_DIR=$PWD
TOOL_DIR=$CURR_DIR/../../tool
TOOL_NAME=xml-solve
TOOL_CMD=$TOOL_DIR/$TOOL_NAME
FILE_1=$CURR_DIR/xml_optimisation_1_analytic.xml

$TOOL_CMD $CURR_DIR/xml_optimisation_1_analytic.xml
$TOOL_CMD $CURR_DIR/xml_optimisation_1_cr.xml
$TOOL_CMD $CURR_DIR/xml_solve_1_cr.xml
$TOOL_CMD $CURR_DIR/xml_solve_1_analytic.xml
#../../tool/xml-solve $BASE_DIR/xml_optimisation_1_analytic.xlm
#../../tool/xml-solve xml_optimisation_1_cr.xlm
#../../tool/xml-solve xml_solve_1_analytic.xlm
#../../tool/xml-solve xml_solve_1_cr.xlm