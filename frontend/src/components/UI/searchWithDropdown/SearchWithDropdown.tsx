import { Dropdown, DropdownItem, Typography, Search } from "@/components/UI"
import { useUserStore } from "@/state/user"

import styles from "./SearchWithDropdown.module.css"
import { useEffect, useState } from "react"

export interface DropdownData {
    id: number
    name: string
    login?: string
}

interface SearchWithDropdownProps {
    setPrompt: (value: string) => void
    onClickDropdownItem: (data: DropdownData) => void
    data: DropdownData[]
}

export const SearchWithDropdown = ({
    setPrompt,
    onClickDropdownItem,
    data
}: SearchWithDropdownProps): JSX.Element => {
    const [input, setInput] = useState("")
    const [dropdownState, setDropdownState] = useState(false)
    const [currentUser, setCurrentUser] = useState<DropdownData | null>(null)
    const currentUserId = useUserStore((state) => state.data?.id)

    const setPromptValue = (value: string) => {
        setPrompt(value)
        setInput(value)
    }

    useEffect(() => {
        console.log("currentUser: " + currentUser)
        if (currentUser === null) {
            return
        }
        onClickDropdownItem({
            id: currentUser.id,
            name: currentUser.name,
            login: currentUser.login
        })
        setDropdownState(false)
    }, [currentUser])

    useEffect(() => {
        console.log("input: " + input)
        if (input === "" || data.length === 0) {
            setDropdownState(false)
            return
        }
        setDropdownState(true)
    }, [data, input])

    return (
        <Search
            setPrompt={setPromptValue}
            className={styles.search}
            onFocus={() => setDropdownState(true)}
            onBlur={() => setTimeout(() => setDropdownState(false), 300)}
        >
            <Dropdown dropdownState={dropdownState} className={styles.dropdown}>
                {data.map((user) => {
                    if (user.id == currentUserId) return
                    return (
                        <DropdownItem
                            key={user.id}
                            className={styles.dropdown_item}
                            onClick={() => {console.log("HI"); setCurrentUser(user)}}
                        >
                            <Typography tag="span" variant="text_tiny" className={styles.name}><>
                                {user.name}
                            </></Typography>
                            { user.login &&
                                <Typography tag="span" variant="text_tiny" className={styles.login}>
                                    {` (@${user.login})`}
                                </Typography>
                            }
                        </DropdownItem>
                )})}
            </Dropdown>
        </Search>
    )
}
