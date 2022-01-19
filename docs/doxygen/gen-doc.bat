@REM genarete xml documentaion 
doxygen

@REM convert to markdown
moxygen -c -h -o "md" xml
