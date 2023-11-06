//
// Created by Baptiste Audugé on 04/10/2023.
//

#include "../include/Grid.h"
#include <vector>
#include <cassert>
#include <iostream>

namespace tomographie {

    Grid::Grid (int n, int m, GridColors default_color) {
        grid = std::vector(n, std::vector(m, default_color));
        size = std::make_pair(n, m);
    }

    bool Grid::is_empty_or_match_line(int line, int i, int j, GridColors color) {
        assert(i <= j);
        for (; i < j; ++i) {
            if (grid[line][i] != EMPTY && grid[line][i] != color) return false;
        }
        return true;
    }

    bool Grid::is_empty_or_match_col(int col, int i, int j, GridColors color) {
        assert(i <= j);
        for (; i < j; ++i) {
            if (grid[i][col] != EMPTY && grid[i][col] != color) return false;
        }
        return true;
    }

     bool Grid::line_can_contain_sequence(int j, int l, Sequence &seq) {
         assert(l >= 0);
         if (!l) return true;
         if (j < seq[l-1] - 1) return false;
         if (j == seq[l-1] - 1) return l == 1;
         return (Grid::line_can_contain_sequence(j - seq[l - 1] - 1, l - 1, seq)
         || Grid::line_can_contain_sequence(j - 1, l, seq));
     }


    bool Grid::line_can_contain_sequence_gen(int line, int j, int l, Sequence &seq) {
        assert(l >= 0);
        if (!l) return is_empty_or_match_line(line, 0, j + 1, WHITE);
        if (j < seq[l-1] - 1) return false;
        if (j == seq[l-1] - 1) {
            return l == 1 && is_empty_or_match_line(line, 0, j + 1, BLACK);
        }
        return (
                (is_empty_or_match_line(line, j - seq[l-1] + 1, j + 1 , BLACK)
                // Checking if the separator (WHITE) is available
                && is_empty_or_match_line(line, j - seq[l - 1], j - seq[l - 1] + 1, WHITE)
                && line_can_contain_sequence_gen(line, j - seq[l - 1] - 1, l - 1, seq))
                ||
                (is_empty_or_match_line(line, j, j + 1, WHITE)
                && line_can_contain_sequence_gen(line, j - 1, l, seq))
                );
    }

    bool Grid::column_can_contain_sequence_gen(int column, int j, int l, Sequence &seq) {
        assert(l >= 0);
        if (!l) return is_empty_or_match_col(column, 0, j + 1, WHITE);
        if (j < seq[l-1] - 1) return false;
        if (j == seq[l-1] - 1) {
            return l == 1 && is_empty_or_match_col(column, 0, j + 1, BLACK);
        }
        return (
                (is_empty_or_match_col(column, j - seq[l-1] + 1, j + 1 , BLACK)
                 // Checking if the separator (WHITE) is available
                 && is_empty_or_match_col(column, j - seq[l - 1], j - seq[l - 1] + 1, WHITE)
                 && column_can_contain_sequence_gen(column, j - seq[l - 1] - 1, l - 1, seq))
                ||
                (is_empty_or_match_col(column, j, j + 1, WHITE)
                 && column_can_contain_sequence_gen(column, j - 1, l, seq))
        );
    }


    GridSize Grid::get_size() {
        return size;
    }

    void Grid::set_cell(int line, int col, GridColors value) {
        assert(line < size.first && col < size.second);
        grid[line][col] = value;
    }

    GridColors Grid::get_cell(int line, int col) {
        return grid[line][col];
    }


    void Grid::print_grid() {
        for (auto &line: grid) {
            for (auto &cell: line) {
                if (cell == EMPTY) {
                    std::cout << std::endl << "Error: You are trying to print a Grid who's not full ..." << std::endl;
                    return;
                }
                if (cell == BLACK)
                    std::cout << BLACK_SYMBOL;
                if (cell == WHITE)
                    std::cout << WHITE_SYMBOL;
            }
            std::cout << std::endl;
        }
    }


} // tomographie