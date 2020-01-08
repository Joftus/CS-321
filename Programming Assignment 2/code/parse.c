#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmd.h"

char* parse_rest(int index, char* in) {
    char *out;
	index ++;
	out = malloc(32 - index);
    for (int a = 0; a < 32 - index; a++) {
		out[a] = in[a + index];
	}
	/*
	printf("parse_rest: %s\n", out);
	printf("parse_rest start: %d\n", index);
	printf("parse_rest size: %lu\n", sizeof(out));
	*/
    return out;
}

int parse_cmd(char *in) {
	// printf("parse_cmd: %s\n", in);
    switch (in[0]) {
        case '0':
            switch (in[1]) {
                case '1':
                    switch (in[2]) {
						case '1':
							switch (in[3]) {
								case '1':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '1':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '1':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '1':
																							{
																								//printf("%s\n", "PRNT");
																								PRNT(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "STURH");
																								STURH(parse_rest(10, in));
																								return 1;
																							}
																					}
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "LDURH");
																								LDURH(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
							}
                        case '0':
                            switch (in[3]) {
                                case '1':
                                    switch (in[4]) {
                                        case '0':
                                            switch (in[5]) {
                                                case '1':
                                                    switch (in[6]) {
                                                        case '0':
                                                            switch (in[7]) {
                                                                case '0':
																	{
																		//printf("%s\n", "B.cond");
																		BCond(parse_rest(7, in));
																		return 1;
																	}
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
                case '0':
                    switch (in[2]) {
						case '1':
							switch (in[3]) {
								case '1':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "STURB");
																								STURB(parse_rest(10, in));
																								return 1;
																							}
																					}
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								printf("%s\n", "LDURB");
																								// LDURB(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
							}
                        case '0':
                            switch (in[3]) {
                                case '1':
                                    switch (in[4]) {
                                        case '0':
                                            switch (in[5]) {
                                                case '1':
													{
														// printf("%s\n", "B");
														B(parse_rest(5, in));
														return 1;
													}
                                            }
                                    }
                            }
                    }
            }
        case '1':
            switch (in[1]) {
				case '1':
					switch (in[2]) {
						case '1':
							switch (in[3]) {
								case '0':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "SUBS");
																								SUBS(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
								case '1':
									switch (in[4]) {
										case '0':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					{
																						// printf("%s\n", "SUBIS");
																						SUBIS(parse_rest(9, in));
																						return 1;
																					}
																			}
																	}
															}
													}
											}
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "STUR");
																								STUR(parse_rest(10, in));
																								return 1;
																							}
																					}
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "LDUR");
																								LDUR(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
												case '1':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '1':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								//printf("%s\n", "PRNL");
																								PRNL(parse_rest(10, in));
																								return 1;
																							}
																					}
																				case '1':
																					switch (in[10]) {
																						case '1':
																							{
																								//printf("%s\n", "HALT");
																								HALT(parse_rest(10, in));
																								return 1;
																							}
																						case '0':
																							{
																								//printf("%s\n", "DUMP");
																								DUMP();
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
							}
						case '0':
							switch (in[3]) {
								case '0':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "SUB");
																								SUB(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "EOR");
																								EOR(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
								case '1':
									switch (in[4]) {
										case '0':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					{
																						// printf("%s\n", "SUBI");
																						SUBI(parse_rest(9, in));
																						return 1;
																					}
																			}
																	}
															}
														case '1':
															switch (in[7]) {
																case '1':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "LSR");
																								LSR(parse_rest(10, in));
																								return 1;
																							}
																						case '1':
																							{
																								//printf("%s\n", "LSL");
																								LSL(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					{
																						//printf("%s\n", "EORI");
																						EORI(parse_rest(9, in));
																						return 1;
																					}
																			}
																	}
															}
													}
												case '1':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "BR");
																								BR(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
							}
					}
                case '0':
                    switch (in[2]) {
						case '1':
							switch (in[3]) {
								case '0':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "ORR");
																								ORR(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
									}
								case '1':
									switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "STURW");
																								STURW(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																		case '1':
																			switch (in[9]) {
																				case '0':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "LDURSW");
																								LDURSW(parse_rest(10, in));
																								return 1;
																							}
																					}
																			}
																	}
															}
													}
											}
										case '0':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '0':
																			switch (in[9]) {
																				case '0':
																					{
																						//printf("%s\n", "ORRI");
																						ORRI(parse_rest(9, in));
																						return 1;
																					}
																			}
																	}
															}
													}
												case '1':
													switch (in[6]) {
														case '0':
															switch (in[7]) {
																case '0':
																	{
																		// printf("%s\n", "CBZ");
																		CBZ(parse_rest(7, in));
																		return 1;
																	}
																case '1':
																	{
																		// printf("%s\n", "CBNZ");
																		CBNZ(parse_rest(7, in));
																		return 1;
																	}
															}
													}
											}
									}
							}
                        case '0':
                            switch (in[3]) {
                                case '1':
                                    switch (in[4]) {
										case '1':
											switch (in[5]) {
												case '0':
													switch (in[6]) {
														case '1':
															switch (in[7]) {
																case '0':
																	switch (in[8]) {
																		case '1':
																			switch (in[9]) {
																				case '1':
																					switch (in[10]) {
																						case '0':
																							switch (in[11]) {
																								case '0':
																									switch (in[12]) {
																										case '0':
																											switch (in[13]) {
																												case '0':
																													switch (in[14]) {
																														case '0':
																															switch (in[15]) {
																																case '1':
																																	switch (in[16]) {
																																		case '1':
																																			{
																																				// printf("%s\n", "UDIV");
																																				UDIV(parse_rest(16, in));
																																				return 1;
																																			}
																																		case '0':
																																			{
																																				// printf("%s\n", "SDIV");
																																				SDIV(parse_rest(10, in));
																																				return 1;
																																			}
																																	}
																															}
																													}
																											}
																									}
																							}
																					}
																			}
																	}
																case '1':
																	switch (in[8]) {
																		case '1':
																			switch (in[9]) {
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "UMULH");
																								UMULH(parse_rest(11, in));
																								return 1;
																							}
																					}
																			}
																		case '0':
																			switch (in[9]) {
																				case '1':
																					switch (in[10]) {
																						case '0':
																							{
																								// printf("%s\n", "SMULH");
																								SMULH(parse_rest(10, in));
																								return 1;
																							}
																					}
																				case '0':
																					switch (in[10]) {
																						case '0':
																							// Start of extra
																							switch (in[11]) {
																								case '0':
																									switch (in[12]) {
																										case '1':
																											switch (in[13]) {
																												case '1':
																													switch (in[14]) {
																														case '1':
																															switch (in[15]) {
																																case '1':
																																	switch (in[16]) {
																																		case '1':
																																			{
																																				// printf("%s\n", "MUL");
																																				MUL(parse_rest(16, in));
																																				return 1;
																																			}
																																	}
																															}
																													}
																											}
																									}
																							}
																					}
																			}
																	}
															}
													}
											}
                                        case '0':
                                            switch (in[5]){
                                                case '1':
													{
                                                    	// printf("%s\n", "BL");
                                                    	BL(parse_rest(5, in));
														return 1;
													}
                                                case '0':
                                                    switch (in[6]) {
                                                        case '1':
                                                            switch (in[7]) {
                                                                case '0':
                                                                    switch (in[8]) {
                                                                        case '0':
                                                                            switch (in[9]) {
                                                                                case '0':
																					{
																						// printf("%s\n", "ANDI");
																						ANDI(parse_rest(9, in));
																						return 1;
																					}
                                                                            }
                                                                    }
                                                            }
                                                        case '0':
                                                            switch (in[7]) {
                                                                case '1':
                                                                    switch (in[8]) {
                                                                        case '0':
                                                                            switch (in[9]) {
                                                                                case '0':
																					{
																						// printf("%s\n", "ADDI");
																						ADDI(parse_rest(9, in));
																						return 1;
																					}
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                                case '0':
                                    switch (in[4]) {
                                        case '1':
                                            switch (in[5]) {
                                                case '0':
                                                    switch (in[6]) {
                                                        case '1':
                                                            switch (in[7]){
                                                                case '0': 
                                                                    switch (in[8]) {
                                                                        case '0':
                                                                            switch (in[9]) {
                                                                                case '0':
                                                                                    switch (in[10]) {
                                                                                        case '0':
																							{
																								// printf("%s\n", "AND");
																								AND(parse_rest(10, in));
																								return 1;
																							}
                                                                                    }
                                                                            }
                                                                    }
                                                                case '1':
                                                                    switch (in[8]) {
                                                                        case '0':
                                                                            switch (in[9]){
                                                                                case '0':
                                                                                    switch (in[10]) {
                                                                                        case '0':
																							{
                                                                                            	// printf("%s\n", "ADD");
                                                                                            	ADD(parse_rest(10, in));
																								return 1;
																							}
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
            }
    }
	// DUMP();
    return 0;
}