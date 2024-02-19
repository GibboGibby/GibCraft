#include "WorldGen.h"

static FastNoise BiomeGenerator(8213);

void SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level, float real_x, float real_z)
{
    BiomeGenerator.SetNoiseType(FastNoise::Simplex);
    //std::cout << "y level for chunk is - " << y_level << std::endl;

    for (int i = 0; i < y_level; i++)
    {
        if (i > 1)
        {
            if (true)
            {
                if (i >= y_level - 1)
                {
                    chunk->SetBlock(BlockType::GRASS, glm::vec3(x, i, z));
                }

                else if (i >= y_level - 5)
                {
                    chunk->SetBlock(BlockType::DIRT, glm::vec3(x, i, z));
                }

                else
                {
                    chunk->SetBlock(BlockType::STONE, glm::vec3(x, i, z));
                }
            }

            else if (false)
            {
                if (i >= y_level - 6)
                {
                    chunk->SetBlock(BlockType::UNKNOWN, glm::vec3(x, i, z));
                }

                else
                {
                    chunk->SetBlock(BlockType::UNKNOWN, glm::vec3(x, i, z));
                }
            }
        }

        else
        {
            chunk->SetBlock(BlockType::COBBLESTONE, glm::vec3(x, i, z));
        }
    }

    return;
}

void AddWaterBlocks(Chunk* chunk, const int water_min, const int water_max)
{
    /*
    Generates water in the areas needed inside of the chunk
    */

    Random water_gen(chunk->pPosition.x + chunk->pPosition.y);
    ChunkDataTypePtr chunk_data = &chunk->pChunkContents;

    for (int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for (int y = water_min; y < water_max + 6; y++)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; z++)
            {
                Block* block = &chunk_data->at(x).at(y).at(z);

                if (block->type == BlockType::AIR)
                {
                    if (y > water_min && y < water_max)
                    {
                        BiomeGenerator.SetNoiseType(FastNoise::Simplex);
                        //float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                        //float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;

                        chunk_data->at(x).at(y).at(z).type = BlockType::UNKNOWN;
                        /*
                        if (x > 0 && y < water_max - 1)
                        {
                            if (chunk_data->at(x - 1).at(y).at(z).IsOpaque())
                            {
                                chunk_data->at(x - 1).at(y).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }

                        if (x < CHUNK_SIZE_X - 1 && y < water_max - 1)
                        {
                            if (chunk_data->at(x + 1).at(y).at(z).IsOpaque())
                            {
                                chunk_data->at(x + 1).at(y).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }

                        if (z > 0 && y < water_max - 1)
                        {
                            if (chunk_data->at(x).at(y).at(z - 1).IsOpaque())
                            {
                                chunk_data->at(x).at(y).at(z - 1).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }

                        if (z < CHUNK_SIZE_Z - 1 && y < water_max - 1)
                        {
                            if (chunk_data->at(x).at(y).at(z + 1).IsOpaque())
                            {
                                chunk_data->at(x).at(y).at(z + 1).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }

                        if (y > 0)
                        {
                            if (chunk_data->at(x).at(y - 1).at(z).IsOpaque())
                            {
                                chunk_data->at(x).at(y - 1).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }

                        if (y < water_max)
                        {
                            if (chunk_data->at(x).at(y + 1).at(z).IsOpaque())
                            {
                                chunk_data->at(x).at(y + 1).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                            }
                        }
                        */
                    }
                }

                else if (block->type != BlockType::AIR && y > water_max - 2 && y < water_max + 4)
                {
                    chunk_data->at(x).at(y).at(z) = { BlockType::UNKNOWN };
                }
            }
        }
    }
}

void WorldGen::GenerateChunk(Chunk* chunk, const int WorldSeed)
{
    int water_min = 2;
    int water_max = 72;

    static FastNoise WorldGenerator(WorldSeed);

    // Set the chunk state
    WorldGenerator.SetNoiseType(FastNoise::SimplexFractal);

    WorldGenerator.SetFrequency(0.006);
    WorldGenerator.SetFractalOctaves(5);
    WorldGenerator.SetFractalLacunarity(2.0f);

    int generated_x = 0;
    int generated_y = 0;
    int generated_z = 0;

    // Generates the world using perlin noise to generate a height map

    for (int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for (int z = 0; z < CHUNK_SIZE_Z; z++)
        {
            int real_x = x + chunk->pPosition.x * CHUNK_SIZE_X;
            int real_z = z + chunk->pPosition.z * CHUNK_SIZE_Z;

            float height_at = 0.0f;


            if (true)
            {
                height_at = (WorldGenerator.GetNoise(real_x / 2.0f, real_z / 2.0f));
            }

            // Generate the biome
            float column_noise = BiomeGenerator.GetNoise(real_x, real_z);
            column_noise = ((column_noise + 1.0f) / 2) * 200;

            //Biome biome = GetBiome(column_noise);
            //chunk->p_BiomeMap[x][z] = biome;

            generated_x = x;
            generated_z = z;

            generated_y = ((height_at + 1.0f) / 2) * 240;

            if (generated_y >= CHUNK_SIZE_Y)
            {
                generated_y = CHUNK_SIZE_Y - 2;
            }

            chunk->p_HeightMap[x][z] = static_cast<uint8_t>(generated_y);
            SetVerticalBlocks(chunk, generated_x, generated_z, generated_y, real_x, real_z);
        }
    }

    AddWaterBlocks(chunk, water_min, water_max);
}


