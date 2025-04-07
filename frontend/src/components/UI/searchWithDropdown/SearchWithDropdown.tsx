import { Dropdown, DropdownItem, Typography, Search } from "@/components/UI"

import styles from "./SearchWithDropdown.module.css"
import { useEffect, useState } from "react"

export interface DropdownData {
    id: number
    name: string
    login?: string
}

interface SearchWithDropdownProps {
    setPrompt: (value: string) => void
    data: DropdownData[]
}

export const SearchWithDropdown = ({
    setPrompt,
    data
}: SearchWithDropdownProps): JSX.Element => {
    const [input, setInput] = useState("")
    const [dropdownState, setDropdownState] = useState(false)

    const setPromptValue = (value: string) => {
        setPrompt(value)
        setInput(value)
    }

    useEffect(() => {
        if (input === "") {
            setDropdownState(false)
            return
        } else if (data.length === 0) {
            setDropdownState(false)
            return
        }
        setDropdownState(true)
    }, [data, input])

    return (
        <Search setPrompt={setPromptValue} className={styles.search}>
            <Dropdown dropdownState={dropdownState} className={styles.dropdown}>
                {data.map((user) => (
                    <DropdownItem key={user.id} className={styles.dropdown_item}>
                        <Typography tag="span" variant="text_tiny" className={styles.name}><>
                            {user.name}
                        </></Typography>
                        { user.login &&
                            <Typography tag="span" variant="text_tiny" className={styles.login}>
                                {` (@${user.login})`}
                            </Typography>
                        }
                    </DropdownItem>
                ))}
            </Dropdown>
        </Search>
    )
}
