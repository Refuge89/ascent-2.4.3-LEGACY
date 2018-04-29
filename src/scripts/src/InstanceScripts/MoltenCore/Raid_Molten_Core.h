/*
 * Summitemu Scripts - Molten Core Defines
 * Copyright (C) 2010 Tbcgaming <http://tbcgaming.com/>
 */
 
 /***Molten Core Boss Script
        Creature & Spell Defines
                                                        **/
                                                        
#define BOSS_LUCIFRON                           12118
#define BOSS_MAGMADAR                           11982
#define BOSS_GEHENNAS                           12259
#define BOSS_GARR                                       12057
#define BOSS_SHAZZRAH                           12264
#define BOSS_GEDDON                                     12056
#define BOSS_GOLEMAGG                           11988
#define BOSS_SULFURON                           12098
#define BOSS_MAJORDOMO                          12018
#define BOSS_RAGNAROS                           11502

#define LUCIFRON_IMPENDING_DOOM         19702
#define LUCIFRON_LUCIFRONS_CURSE        19703
#define LUCIFRON_SHADOW_SHOCK           19460
#define MAGMADAR_MAGMA_SPIT                     19449
#define MAGMADAR_LAVA_BREATH            19272
#define MAGMADAR_PANIC                          19408
#define MAGMADAR_LAVA_BOMB                      19411
#define MAGMADAR_LAVA_BOMB_DEBUFF       19428
#define MAGMADAR_ENRAGE                         19451
#define GEHENNAS_SHADOW_BOLT            19729
#define GEHENNAS_RAIN_OF_FIRE           19717
#define GEHENNAS_CURSE                          19716
#define GARR_ANTIMAGIC_PULSE            19492
#define GARR_MAGMA_SHACKLES                     19496
#define SHAZZRAH_ARCANE_EXPLOSION       19712
#define SHAZZRAH_CURSE                          19713
#define SHAZZRAH_MAGIC_GROUNDING        19714
#define SHAZZRAH_COUNTERSPELL           19715
#define SHAZZRAH_BLINK                          21655
#define GEDDON_INFERNO                          19695
#define GEDDON_IGNITE_MANA                      19659
#define GEDDON_LIVING_BOMB                      20475
#define GEDDON_ARMAGEDDON                       20479
#define GOLEMAGG_TRUST                          20553
#define GOLEMAGG_MAGMA_SPLASH           13880 //13879
#define GOLEMAGG_PYROBLAST                      20228
#define GOLEMAGG_EARTHQUAKE                     19798
#define SULFURON_DEMORALIZING_SHOUT     19778
#define SULFURON_INSPIRE                        19779
#define SULFURON_HAND_OF_RAGNAROS       19780
#define SULFURON_FLAME_SPEAR            19781
#define MAJORDOMO_AEGIS_OF_RAGNAROS 20620
#define MAJORDOMO_MAGIC_REFLECTION      20619
#define MAJORDOMO_DAMAGE_SHIELD         21075
#define MAJORDOMO_TELEPORT                      20618
#define MAJORDOMO_BLAST_WAVE            20229
#define RAGNAROS_WRATH_OF_RAGNAROS      20566
#define RAGNAROS_HAND_OF_RAGNAROS       19780
#define RAGNAROS_ERUPTION                       17731
#define RAGNAROS_ELEMENTAL_FIRE         20564
#define RAGNAROS_MAGMA_BLAST            20565
#define RAGNAROS_MELT_WEAPON            21388



 /***Molten Core Trash Script
        Creature & Spell Defines
                                                        **/
                                                        
#define CREATURE_FLAMEWAKER_PROT        12119
#define FLAMEWAKER_PROT_CLEAVE          20691
#define FLAMEWAKER_PROT_DM                      20740

#define CREATURE_ACHOUND                        11673
#define ACHOUND_LAVA_BREATH                     19272
#define ACHOUND_VICIOUS_BITE            19319
#define ACHOUND_GROUND_STOMP            19364
#define ACHOUND_ANCIENT_DREAD           19365
#define ACHOUND_ANCIENT_DESPAIR         19369
#define ACHOUND_CAUTERIZING_FLAMES      19366
#define ACHOUND_WITHERING_HEAT          19367
#define ACHOUND_ANCIENT_HYSTERIA        19372

#define CREATURE_COREHOUND                      11671
#define COREHOUND_SERRATEDBITE          19771

#define CREATURE_FLAMEWAKER                     11661
#define FLAMEWAKER_SUNDER_ARMOR         15502
#define FLAMEWAKER_FISTOFRAG            20277
#define FLAMEWAKER_STRIKE                       19730

#define CREATURE_FIRESWORN                      12099
#define FIRESWORN_IMMOLATE                      20294
#define FIRESWORN_ERUPTION                      19497
#define FIRESWORN_SEPANXIETY            23492

#define CREATURE_FLAMEWALKERPRIEST      11662
#define FLAMEWAKERPRIEST_DMENDING       19775
#define FLAMEWAKERPRIEST_DSTRIKE        19777
#define FLAMEWAKERPRIEST_IMMOLATE       20294
#define FLAMEWAKERPRIEST_SWPAIN         19776

#define CREATURE_CORERAGER                      11672
#define CORERAGER_MANGLE                        19820

// Misc Functions
void SpellFunc_ShazzrahBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType);