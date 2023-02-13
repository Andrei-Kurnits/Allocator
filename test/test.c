// clang-format off
#include "allocator.h"
#include "allocator_debug.h"
#include "port.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdarg.h>
#include "cmocka.h"
// clang-format on

// Mocks for port funcions:
void allocatorPortInit(void)
{
    function_called();
}
void allocatorPortEnterCriticalSection(void)
{
    function_called();
}
void allocatorPortExitCriticalSection(void)
{
    function_called();
}

static void testSimpleAllocation(void** state)
{
    (void)state; /* unused */
    expect_function_call(allocatorPortInit);
    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    AllocatorInit();

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    int result;
    void* page = AllocatorAlloc(&result);
    assert_int_equal(result, ALLOCATOR_NO_ERROR);
    assert_non_null(page);

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    result = AllocatorDealloc(page);
    assert_int_equal(result, ALLOCATOR_NO_ERROR);
}

static void testCountFreePagesLeft(void** state)
{
    (void)state; /* unused */
    expect_function_call(allocatorPortInit);
    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    AllocatorInit();
    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    int freePages = AllocatorDebugCountFreePages();
    assert_int_equal(freePages, ALLOCATOR_POOL_SIZE);

    int result;
    void* page;
    for (int i = 0; i < ALLOCATOR_POOL_SIZE - 10; i++) {
        expect_function_call(allocatorPortEnterCriticalSection);
        expect_function_call(allocatorPortExitCriticalSection);
        page = AllocatorAlloc(&result);
        assert_int_equal(result, ALLOCATOR_NO_ERROR);
        assert_non_null(page);
    }

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    freePages = AllocatorDebugCountFreePages();
    assert_int_equal(freePages, 10);
}

static void testAllPagesAllocated(void** state)
{
    (void)state; /* unused */
    expect_function_call(allocatorPortInit);
    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    AllocatorInit();

    int result;
    void* page;
    for (int i = 0; i < ALLOCATOR_POOL_SIZE; i++) {
        expect_function_call(allocatorPortEnterCriticalSection);
        expect_function_call(allocatorPortExitCriticalSection);
        page = AllocatorAlloc(&result);
        assert_int_equal(result, ALLOCATOR_NO_ERROR);
        assert_non_null(page);
    }
    void* lastAllocatedPage = page;

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    page = AllocatorAlloc(&result);
    assert_int_equal(result, ALLOCATOR_ERROR_NO_FREE_PAGES);
    assert_null(page);

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    int freePages = AllocatorDebugCountFreePages();
    assert_int_equal(freePages, 0);

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    result = AllocatorDealloc(lastAllocatedPage);
    assert_int_equal(result, ALLOCATOR_NO_ERROR);

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    freePages = AllocatorDebugCountFreePages();
    assert_int_equal(freePages, 1);
}

static void testBadAddress(void** state)
{
    (void)state; /* unused */
    expect_function_call(allocatorPortInit);
    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    AllocatorInit();

    expect_function_call(allocatorPortEnterCriticalSection);
    expect_function_call(allocatorPortExitCriticalSection);
    int result;
    void* page = AllocatorAlloc(&result);
    assert_int_equal(result, ALLOCATOR_NO_ERROR);
    assert_non_null(page);

    result = AllocatorDealloc(AllocatorDebugGetPoolBaseAdress() - 1);
    assert_int_equal(result, ALLOCATOR_ERROR_BAD_ADDRESS);

    result = AllocatorDealloc(page + 1);
    assert_int_equal(result, ALLOCATOR_ERROR_NOT_ALIGNED);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testSimpleAllocation),
        cmocka_unit_test(testCountFreePagesLeft),
        cmocka_unit_test(testAllPagesAllocated),
        cmocka_unit_test(testBadAddress),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}