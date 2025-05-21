import { Button, Input, Label } from "@/components/UI"

import styles from "./Search.module.css"
import { ReactSVG } from "react-svg"
import useInput from "@/hooks/useInput"
import { useEffect } from "react"
import cls from "@/utils/cls"

interface SearchProps extends React.InputHTMLAttributes<HTMLInputElement> {
    setPrompt: (value: string) => void
    className?: string
    children?: React.ReactNode
    getRef?: (ref: React.RefObject<HTMLInputElement>) => void
}

export const Search = ({
    setPrompt,
    className = "",
    children,
    ...props
}: SearchProps): JSX.Element => {
    const [value] = useInput("")

    useEffect(() => {
        const timeout = setTimeout(() => {
            setPrompt(value.value)
        }, 500)
        return () => clearTimeout(timeout)
    }, [value.value])

    return (
        <search className={cls(styles.search, className)}>
            <form action="">
                <Label htmlFor="search" variant="search">
                    <Input
                        id="search"
                        type="search"
                        variant="search"
                        placeholder="Find a friend..."
                        {...value}
                        {...props}
                    />
                </Label>
                <Button className={styles.button} variant="icon">
                    <ReactSVG className={styles.icon} src="/search.svg" />
                </Button>
            </form>
            {children && children}
        </search>
    )
}
