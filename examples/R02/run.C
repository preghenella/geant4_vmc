void
run(const char *what)
{
  gInterpreter->ProcessLine(Form(".x load_%s.C", what));
  gInterpreter->ProcessLine(Form(".x run_%s.C", what));
}
