#ifdef WITHOUT_WHITE
  #pragma message("WITHOUT_WHITE yes")
#else
  #pragma message("WITHOUT_WHITE not")
#endif
#ifdef START_AOF
  #pragma message("START_AOF yes")
#else
  #pragma message("START_AOF not")
#endif
int main()
{  return 0;
}
